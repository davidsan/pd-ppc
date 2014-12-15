/*
 * CULTO    : Concurrent UPMC LapTop Orchestra
 *            Exercice PPC - 2014
 *
 * Exercice 3. Création d'un objet PureData de ducking (modification du volume d'un signal par le volume d'un autre signal).
 *
 * author   : ds & vc
 *
 */

#include "duck~.h"

/*
 * Q.5 - Fonction centrale effectuant le calcul
 */
t_int           *duck_tilde_perform(t_int *w){
  t_duck_tilde *x = (t_duck_tilde *)(w[1]);
  t_sample  *in1 =    (t_sample *)(w[2]);
  t_sample  *in2 =    (t_sample *)(w[3]);
  t_sample  *out =    (t_sample *)(w[4]);
  int          n =           (int)(w[5]);
  t_sample f_pan = (x->f_pan<0)?0.0:(x->f_pan>1)?1.0:x->f_pan;

  while (n--) *out++ = (*in1++)*(1-f_pan)+(*in2++)*f_pan;

  return (w+6);
}
/*
 * Q.4 - Ajout de l'objet duck~ à l'arbre de traitement DSP
 */
void            duck_tilde_dsp(t_duck_tilde *x, t_signal **sp){
    dsp_add(duck_tilde_perform, 5, x,
            sp[0]->s_vec, sp[1]->s_vec, sp[2]->s_vec, sp[0]->s_n);
}
/*
 * Q.3 - Libération de la mémoire de l'objet duck~
 */
void            duck_tilde_free(t_duck_tilde *x){
    inlet_free(x->x_in2);
    outlet_free(x->x_out);
}
/*
 * Q.2 - Création d'un nouvel objet duck~
 */
void            *duck_tilde_new(void){

    t_duck_tilde   *x;

    x = (t_duck_tilde *)pd_new(duck_tilde_class);

    x->x_in2 = inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_signal, &s_signal);
    x->x_out = outlet_new(&x->x_obj, &s_signal);
    return (void *)x;
}

/*
 * Q.1 - Chargement en mémoire des objets de type duck~
 */
void            duck_tilde_setup(void){
  duck_tilde_class = class_new(gensym("duck~"),
                                (t_newmethod)duck_tilde_new,
                                (t_method)duck_tilde_free,
                                sizeof(t_duck_tilde),
                                CLASS_DEFAULT,
                                0);

  class_addmethod(duck_tilde_class,
                  (t_method)duck_tilde_dsp,
                  gensym("dsp"),
                  0);

  CLASS_MAINSIGNALIN(duck_tilde_class,
                     t_duck_tilde,
                     f);

}
