  #include        <time.h>
  #include        <string.h>
  #include        "multipouet.h"

  /*
   * Q.4 - Comportement de l'objet en cas de messages reset, set ou bound
   */
  void            multipouet_reset(t_multipouet *x){
      x->i_count = x->i_min;
      post("i_count is %d", x->i_count);
  }

  void            multipouet_set(t_multipouet *x, t_floatarg f){
      x->i_count = f;
      post("i_count is %d", x->i_count);
  }

  void            multipouet_bound(t_multipouet *x, t_floatarg min, t_floatarg max){
      x->i_min = min;
      x->i_max = max;
      post("i_min is %d", x->i_min);
      post("i_max is %d", x->i_max);
  }

  /*
   * Q.3 - Comportement en cas de message bang
   */
  void            multipouet_bang(t_multipouet *x){
      x->i_count = x->i_count + x->step;
      char pouet[1024];
      strcpy(pouet,"");
      int i;
      for(i=0 ; i<x->i_count ; i++){
        strcat(pouet, "pouet ");
      }
      outlet_symbol(x->p_out, gensym(pouet));
      if(x->i_count >= x->i_max){
        outlet_symbol(x->b_out, gensym("bang"));
        x->i_count=x->i_min;
      }
  }

  /*
   * Q.2 - Création d'un nouvel objet multipouet
   */
  void            *multipouet_new(t_symbol *s, int argc, t_atom *argv){
      t_multipouet   *m;
      m = (t_multipouet *)pd_new(multipouet_class);

      // initialize i_min
      // with a default value

      m->step = 0;
      m->i_min = 0;
      m->i_max = 0;

      // initialize step, i_min and i_max
      // from args if args are present

      if(argc >= 1){
        m->step = atom_getfloat(argv);
        if(argc >= 2){
          m->i_min = atom_getfloat(argv+1);
          if(argc >= 3){
              m->i_max = atom_getfloat(argv+2);
          }else{
              m->i_max = m->i_min;
          }
        }
      }

      // init i_count
      m->i_count = m->i_min;

      // inlets defined left to right
      // step inlet
      floatinlet_new(&m->x_obj, &m->step);

      // bound inlet
      inlet_new(&m->x_obj,
                &m->x_obj.ob_pd,
                gensym("list"),
                gensym("bound"));

      // print outlet
      m->p_out = outlet_new(&m->x_obj, &s_symbol);
      // bang outlet
      m->b_out = outlet_new(&m->x_obj, &s_bang);
      return (void *)m;
  }

  /*
   * Q.1 - Chargement en mémoire des objets de type multipouet
   */
  void            multipouet_setup(void){
      multipouet_class = class_new(gensym("multipouet"),
                                (t_newmethod)multipouet_new,
                                0,
                                sizeof(t_multipouet),
                                CLASS_DEFAULT,
                                A_GIMME,
                                0);
      class_addbang(multipouet_class, multipouet_bang);
      class_addmethod(multipouet_class,
                      (t_method)multipouet_reset,
                      gensym("reset"),
                      0);
      class_addmethod(multipouet_class,
                      (t_method)multipouet_set,
                      gensym("set"),
                      A_DEFFLOAT,
                      0);
      class_addmethod(multipouet_class,
                      (t_method)multipouet_bound,
                      gensym("bound"),
                      A_DEFFLOAT,
                      A_DEFFLOAT,
                      0);
  }
