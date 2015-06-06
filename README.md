Factor
======
Integer factorization using Quadratic Sieve.

Dependencies
============
You will need the gmp package containing GNU MP for arbitrary precision
arithmetic and also the MPFR library for multiple-precision floating-point
arithmetic.

Examples
========
The parameters defined in `config.h` can greatly change the integer
factorization speed to better or worse.

    $ make
    $ ./factor 32000104328923478923489234890234890234890890234
    32000104328923478923489234890234890234890890234: 2 3 191 248041 2924794690724209 38490000553137727468841
    $ ./factor 51884496199884033707060070710306434395690951779
    51884496199884033707060070710306434395690951779: 10906804661271099530111 4757075771616259690952989


Timed Comparison
=================
If you are running GNU/Linux there is a program `factor` in coreutils that can
be used for comparison. It uses Pollard-Rho as a integer factorization
technique but it does not work well on numbers which are the product of two large
prime numbers as seen below.

Quadratic Sieve
---------------
    $ time ./factor 32000104328923478923489234890234890234890890234
    32000104328923478923489234890234890234890890234: 2 3 191 248041 2924794690724209 38490000553137727468841

    real    0m0.861s
    user    0m0.848s
    sys     0m0.012s

    $ time ./factor 51884496199884033707060070710306434395690951779
    51884496199884033707060070710306434395690951779: 10906804661271099530111 4757075771616259690952989

    real    1m10.573s
    user    1m10.477s
    sys     0m0.022s

Pollard-Rho
-----------
    $ time factor 32000104328923478923489234890234890234890890234
    32000104328923478923489234890234890234890890234: 2 3 191 248041 2924794690724209 38490000553137727468841

    real    0m21.231s
    user    0m21.212s
    sys     0m0.002s

    $ time factor 51884496199884033707060070710306434395690951779

    // aborted after over 1 hour
