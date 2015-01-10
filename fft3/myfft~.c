#include <stdlib.h>
#include "myfft_fft.c"
#include "myfft~.h"

/*
 * Q.5 - Fonction centrale effectuant le calcul
 */
t_int           *myfft_tilde_perform(t_int *w)
{
	t_myfft_tilde *x = (t_myfft_tilde *)(w[1]);
	t_sample  *in1 =    (t_sample *)(w[2]);
	t_sample  *out =    (t_sample *)(w[3]);
	int          n =           (int)(w[4]);
	int          i = 					 0;

	for (i = 0; i < n; i++) {
		out[i] = (*in1++) * x->window[i];
	}

	rdft(1024, 1, out, x->bitshuffle, x->weighting);

	return (w+5);
}

/*
 * Q.4 - Ajout de l'objet myfft~ à l'arbre de traitement DSP
 */
void            myfft_tilde_dsp(t_myfft_tilde *x, t_signal **sp)
{
	dsp_add(myfft_tilde_perform, 4, x,
          sp[0]->s_vec, sp[1]->s_vec, sp[0]->s_n);
}

/*
 * Q.3 - Libération de la mémoire de l'objet myfft~
 */
void            myfft_tilde_free(t_myfft_tilde *x)
{
  free(x->window);
  free(x->bitshuffle);
  free(x->weighting);
	outlet_free(x->x_out);
}

/*
 * Q.2 - Création d'un nouvel objet myfft~
 */
void            *myfft_tilde_new(void)
{
  	t_myfft_tilde    *m;
    m = (t_myfft_tilde *)pd_new(myfft_tilde_class);

    m->x_out = outlet_new(&m->x_obj, &s_signal);
  	m->window = malloc(1024 * sizeof * m->window);
  	m->bitshuffle = malloc(2048 * sizeof * m->bitshuffle);
  	m->weighting = malloc(2048 * sizeof * m->weighting);

  	int i;
  	for (i = 0; i < 1024; i++) {
  		m->window[i] = (float) (0.54-0.46*(cos (2*PI * i/1024)));
  	}

		init_rdft(1024, m->bitshuffle, m->weighting);
    return (void *)m;
}

/*
 * Q.1 - Chargement en mémoire des objets de type myfft~
 */
void            myfft_tilde_setup(void)
{
	myfft_tilde_class = class_new(gensym("myfft~"),
                              (t_newmethod)myfft_tilde_new,
                              (t_method) myfft_tilde_free,
                              sizeof(t_myfft_tilde),
                              CLASS_DEFAULT,
                              0);

  class_addmethod(myfft_tilde_class,
        (t_method)myfft_tilde_dsp,
        gensym("dsp"),
        0);

	CLASS_MAINSIGNALIN(myfft_tilde_class,
        t_myfft_tilde,
        f);
}
