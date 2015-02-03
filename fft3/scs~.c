#include <math.h>
#include <stdlib.h>
#include "scs~.h"
#include "myfft_fft.c"

#define SIZE 2048
float distance_euclidienne(float x, float y)
{
	return sqrtf(x * x + y * y);
}

void scs_tilde_messages(t_scs_tilde *x, t_floatarg n, t_floatarg p)
{
	x->autonorm = n;
	x->bypass   = p;
}

/*
 * Q.5 - Fonction centrale effectuant le calcul
 */
t_int           *scs_tilde_perform(t_int *w)
{
	t_scs_tilde *x = (t_scs_tilde *)(w[1]);
	t_sample   *in1 =     (t_sample *)(w[2]);
	t_sample   *in2 =     (t_sample *)(w[3]);
	t_sample   *out =     (t_sample *)(w[4]);
	int           n =            (int)(w[5]);
	int 	   	  i =                      0;
	int 	   	  j =                      0;
	t_sample  *dup1, *dup2, *tmp1, *tmp2, *res;
	t_sample     a1, a2, b1, b2;
	float    ampSum;
	float   freqSum;
	float    factor;

	// Allocation
	dup1 = malloc (n * sizeof * dup1);
	dup2 = malloc (n * sizeof * dup2);
	tmp1 = malloc (n * sizeof * tmp1);
	tmp2 = malloc (n * sizeof * tmp2);
	res  = malloc (n * sizeof * res);

	// Renvoi directement le in1 en sortie
	if (x->bypass == 1) {
		for(i = 0; i<n; i++) {
			out[i] = in1[i];
		}
		if (x->autonorm == 1) {
			for(i = 0; i < n; i++){
				out[i] = out[i] / 250;
			}
		}
		return (w+6);
	}

	// Duplication des inputs in1 et in2 dans dup1 et dup2
	for(i = 0; i<n; i++) {
		dup1[i] = in1[i] * x->window[i];
		dup2[i] = in2[i] * x->window[i];
	}

	// Application de rdft sur dup1 et dup2
	rdft(n, 1, dup1, x->bitshuffle, x->weighting);
	rdft(n, 1, dup2, x->bitshuffle, x->weighting);

	//Conversion des valeurs

	for(i = 0; i<n; i = i+2) {
		a1 = dup1[i];
		b1 = dup1[i+1];
		a2 = dup2[i];
		b2 = dup2[i+1];
		tmp1[i] = distance_euclidienne(a1, b1);
		tmp1[i+1] = -atan2(b1, a1);
		tmp2[i] = distance_euclidienne(a2, b2);
		tmp2[i+1] = -atan2(b2, a2);
	}

	for(i = 0; i<n; i = i + x->shapeWidth * 2) {
		ampSum = 0;
		freqSum = 0;
		for(j = 0; j < (x->shapeWidth) * 2; j = j + 2) {
			ampSum += tmp2[i+j];
			freqSum += tmp1[i+j];
		}
		factor = ampSum / freqSum;
		for(j = 0; j<(x->shapeWidth) * 2; j = j + 2) {
			tmp1[i+j] *= factor;
		}
	}

	for(i = 0; i<n; i = i + 2) {
		res[i] = tmp1[i] * cos(tmp1[i+1]);
		res[i + 1] = -tmp1[i] * sin(tmp1[i+1]);
	}

	rdft(n, -1, res, x->bitshuffle, x->weighting);

	if (x->autonorm == 1) {

		for(i = 0; i < n; i++){
			res[i] = res[i] / 1000;
		}
	}
	for(i = 0; i < n; i++){
		out[i] = res[i];
	}

	free(dup1);
	free(dup2);
	free(tmp1);
	free(tmp2);
	free(res);

	return w+6;
}

/*
 * Q.4 - Ajout de l'objet scs à l'arbre de traitement DSP
 */
void            scs_tilde_dsp(t_scs_tilde *x, t_signal **sp)
{
	dsp_add(scs_tilde_perform, 5, x,
          sp[0]->s_vec, sp[1]->s_vec, sp[2]->s_vec, sp[0]->s_n);
}

/*
 * Q.3 - Libération de la mémoire de l'objet scs~
 */
void            scs_tilde_free(t_scs_tilde *x)
{
	inlet_free(x->x_in2);
	outlet_free(x->x_out);
}

/*
 * Q.2 - Création d'un nouvel objet scs
 */
void            *scs_tilde_new(int argc, t_atom * argv)
{
	int i  = 0;
	t_scs_tilde *m = (t_scs_tilde *)pd_new(scs_tilde_class);
	m->bypass = 0;
	m->autonorm = 1;
	m->shapeWidth = 1;

	// Si argument present
	switch (argc) {
		// Bypass en argument
		case 2 :
			m->bypass = atom_getint(argv + (1 * sizeof(t_atom)));
		case 1 :
			m->autonorm = atom_getint(argv);
		default :
			break;
	}

	m->x_in2 = inlet_new(&m->x_obj, &m->x_obj.ob_pd,
              &s_signal, &s_signal);
	floatinlet_new(&m->x_obj, &m->shapeWidth);
	m->messages = inlet_new(&m->x_obj,
			  &m->x_obj.ob_pd,
			  gensym("list"),
			  gensym("messages"));


  m->x_out = outlet_new(&m->x_obj, &s_signal);
  m->window = malloc(SIZE * sizeof * m->window);
  m->bitshuffle = malloc(SIZE * 2 * sizeof * m->bitshuffle);
  m->weighting = malloc(SIZE * 2 * sizeof * m->weighting);

  for (i = 0; i<SIZE; i++) {
    m->window[i] = (float) (0.54-0.46*(cos (TWOPI * i/SIZE)));
  }

  init_rdft(SIZE, m->bitshuffle, m->weighting);
  return (void *)m;
}

/*
 * Q.1 - Chargement en mémoire des objets de type scs
 */
void            scs_tilde_setup(void)
{
	scs_tilde_class = class_new(gensym("scs~"),
                              (t_newmethod)scs_tilde_new,
                              (t_method) scs_tilde_free, sizeof(t_scs_tilde),
                              CLASS_DEFAULT, A_GIMME, 0);
	class_addmethod(scs_tilde_class, (t_method)scs_tilde_dsp, gensym("dsp"), 0);
	class_addmethod(scs_tilde_class, (t_method)scs_tilde_messages,
					gensym("messages"), A_DEFFLOAT, A_DEFFLOAT, 0);
	CLASS_MAINSIGNALIN(scs_tilde_class, t_scs_tilde, f);
}
