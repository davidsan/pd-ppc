#ifndef _SCS_H_
# define _SCS_H_

# include "m_pd.h"

#ifndef PI
#define PI 3.141592653589793115997963468544185161590576171875
#endif

#ifndef TWOPI
#define TWOPI 6.28318530717958623199592693708837032318115234375
#endif

static t_class      *scs_tilde_class;
typedef struct      _scs_tilde
{
  t_object        x_obj;
  t_sample		f;
  t_inlet 		*x_in2;
  t_inlet         *messages;
  t_outlet		*x_out;
  int             *bitshuffle;
  float           *weighting;
  float           *window;
  t_float			shapeWidth;
  t_int 			bypass;
  t_int 			autonorm;
}
t_scs_tilde;

void            scs_tilde_messages(t_scs_tilde *x, t_floatarg n, t_floatarg p);

t_int           *scs_tilde_perform(t_int *w);

void            scs_tilde_dsp(t_scs_tilde *x, t_signal **sp);

void            scs_tilde_free(t_scs_tilde *x);

void            *scs_tilde_new(int argc, t_atom * argv);

void            scs_tilde_setup(void);

#endif
