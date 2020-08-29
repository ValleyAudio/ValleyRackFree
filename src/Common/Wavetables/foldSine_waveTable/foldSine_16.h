#ifndef WAVETABLE_FOLDSINE_16_H
#define WAVETABLE_FOLDSINE_16_H
#define WAVETABLE_FOLDSINE_16_LENGTH 1024

static long foldSine_16_tableLength = 1024;
static float foldSine_16_waveTable[WAVETABLE_FOLDSINE_16_LENGTH] = {
    0.000000e+00, 8.671976e-02, 1.727829e-01, 2.575380e-01, 3.403438e-01, 4.205746e-01, 4.976248e-01, 5.709137e-01, 6.398902e-01, 7.040371e-01, 7.628747e-01, 8.159650e-01, 8.629147e-01, 9.033782e-01, 9.370603e-01, 9.637182e-01, 
    9.831633e-01, 9.952625e-01, 9.999390e-01, 9.971723e-01, 9.869989e-01, 9.695110e-01, 9.448559e-01, 9.132342e-01, 8.748986e-01, 8.301510e-01, 7.793401e-01, 7.228585e-01, 6.611396e-01, 5.946532e-01, 5.239027e-01, 4.494202e-01, 
    3.717626e-01, 2.915070e-01, 2.092463e-01, 1.255845e-01, 4.113189e-02, -4.349925e-02, -1.276997e-01, -2.108675e-01, -2.924126e-01, -3.717612e-01, -4.483597e-01, -5.216785e-01, -5.912160e-01, -6.565020e-01, -7.171002e-01, -7.726118e-01, 
    -8.226776e-01, -8.669802e-01, -9.052456e-01, -9.372451e-01, -9.627960e-01, -9.817622e-01, -9.940552e-01, -9.996333e-01, -9.985016e-01, -9.907115e-01, -9.763593e-01, -9.555849e-01, -9.285705e-01, -8.955382e-01, -8.567482e-01, -8.124961e-01, 
    -7.631104e-01, -7.089498e-01, -6.503999e-01, -5.878705e-01, -5.217921e-01, -4.526126e-01, -3.807943e-01, -3.068099e-01, -2.311399e-01, -1.542684e-01, -7.668060e-02, 1.140920e-03, 7.871920e-02, 1.555859e-01, 2.312842e-01, 3.053715e-01, 
    3.774220e-01, 4.470293e-01, 5.138081e-01, 5.773967e-01, 6.374585e-01, 6.936836e-01, 7.457903e-01, 7.935257e-01, 8.366675e-01, 8.750238e-01, 9.084340e-01, 9.367689e-01, 9.599307e-01, 9.778529e-01, 9.904998e-01, 9.978660e-01, 
    9.999757e-01, 9.968816e-01, 9.886641e-01, 9.754299e-01, 9.573106e-01, 9.344616e-01, 9.070602e-01, 8.753040e-01, 8.394095e-01, 7.996101e-01, 7.561541e-01, 7.093033e-01, 6.593310e-01, 6.065199e-01, 5.511608e-01, 4.935500e-01, 
    4.339883e-01, 3.727786e-01, 3.102249e-01, 2.466298e-01, 1.822936e-01, 1.175127e-01, 5.257797e-02, -1.222661e-02, -7.662487e-02, -1.403499e-01, -2.031448e-01, -2.647641e-01, -3.249740e-01, -3.835537e-01, -4.402955e-01, -4.950059e-01, 
    -5.475056e-01, -5.976299e-01, -6.452294e-01, -6.901695e-01, -7.323309e-01, -7.716095e-01, -8.079164e-01, -8.411774e-01, -8.713332e-01, -8.983387e-01, -9.221631e-01, -9.427889e-01, -9.602119e-01, -9.744405e-01, -9.854952e-01, -9.934078e-01, 
    -9.982212e-01, -9.999883e-01, -9.987717e-01, -9.946427e-01, -9.876809e-01, -9.779734e-01, -9.656139e-01, -9.507024e-01, -9.333442e-01, -9.136493e-01, -8.917317e-01, -8.677089e-01, -8.417010e-01, -8.138305e-01, -7.842211e-01, -7.529977e-01, 
    -7.202855e-01, -6.862097e-01, -6.508949e-01, -6.144646e-01, -5.770408e-01, -5.387436e-01, -4.996910e-01, -4.599983e-01, -4.197779e-01, -3.791389e-01, -3.381872e-01, -2.970248e-01, -2.557500e-01, -2.144569e-01, -1.732355e-01, -1.321718e-01, 
    -9.134687e-02, -5.083780e-02, -1.071698e-02, 2.894768e-02, 6.809276e-02, 1.066594e-01, 1.445931e-01, 1.818439e-01, 2.183662e-01, 2.541185e-01, 2.890638e-01, 3.231689e-01, 3.564048e-01, 3.887462e-01, 4.201717e-01, 4.506632e-01, 
    4.802066e-01, 5.087905e-01, 5.364071e-01, 5.630516e-01, 5.887217e-01, 6.134183e-01, 6.371446e-01, 6.599062e-01, 6.817110e-01, 7.025690e-01, 7.224922e-01, 7.414944e-01, 7.595909e-01, 7.767987e-01, 7.931361e-01, 8.086227e-01, 
    8.232792e-01, 8.371272e-01, 8.501893e-01, 8.624888e-01, 8.740495e-01, 8.848961e-01, 8.950533e-01, 9.045464e-01, 9.134009e-01, 9.216423e-01, 9.292964e-01, 9.363889e-01, 9.429453e-01, 9.489912e-01, 9.545517e-01, 9.596517e-01, 
    9.643160e-01, 9.685688e-01, 9.724338e-01, 9.759344e-01, 9.790935e-01, 9.819332e-01, 9.844753e-01, 9.867408e-01, 9.887501e-01, 9.905229e-01, 9.920782e-01, 9.934344e-01, 9.946091e-01, 9.956189e-01, 9.964801e-01, 9.972078e-01, 
    9.978166e-01, 9.983200e-01, 9.987310e-01, 9.990616e-01, 9.993230e-01, 9.995256e-01, 9.996789e-01, 9.997917e-01, 9.998718e-01, 9.999262e-01, 9.999611e-01, 9.999819e-01, 9.999930e-01, 9.999980e-01, 9.999997e-01, 1.000000e+00, 
    1.000000e+00, 9.999999e-01, 9.999991e-01, 9.999960e-01, 9.999884e-01, 9.999730e-01, 9.999458e-01, 9.999018e-01, 9.998354e-01, 9.997399e-01, 9.996079e-01, 9.994310e-01, 9.992003e-01, 9.989056e-01, 9.985363e-01, 9.980806e-01, 
    9.975262e-01, 9.968597e-01, 9.960671e-01, 9.951336e-01, 9.940434e-01, 9.927801e-01, 9.913266e-01, 9.896648e-01, 9.877762e-01, 9.856413e-01, 9.832401e-01, 9.805519e-01, 9.775553e-01, 9.742282e-01, 9.705483e-01, 9.664923e-01, 
    9.620368e-01, 9.571577e-01, 9.518305e-01, 9.460305e-01, 9.397325e-01, 9.329113e-01, 9.255412e-01, 9.175966e-01, 9.090519e-01, 8.998813e-01, 8.900593e-01, 8.795606e-01, 8.683600e-01, 8.564329e-01, 8.437550e-01, 8.303029e-01, 
    8.160534e-01, 8.009845e-01, 7.850750e-01, 7.683047e-01, 7.506548e-01, 7.321075e-01, 7.126466e-01, 6.922576e-01, 6.709276e-01, 6.486455e-01, 6.254025e-01, 6.011916e-01, 5.760084e-01, 5.498511e-01, 5.227201e-01, 4.946190e-01, 
    4.655542e-01, 4.355352e-01, 4.045746e-01, 3.726888e-01, 3.398972e-01, 3.062233e-01, 2.716942e-01, 2.363410e-01, 2.001988e-01, 1.633068e-01, 1.257086e-01, 8.745188e-02, 4.858895e-02, 9.176440e-03, -3.072451e-02, -7.104823e-02, 
    -1.117245e-01, -1.526788e-01, -1.938318e-01, -2.351000e-01, -2.763955e-01, -3.176261e-01, -3.586957e-01, -3.995040e-01, -4.399472e-01, -4.799176e-01, -5.193045e-01, -5.579939e-01, -5.958693e-01, -6.328115e-01, -6.686995e-01, -7.034103e-01, 
    -7.368199e-01, -7.688033e-01, -7.992354e-01, -8.279909e-01, -8.549455e-01, -8.799760e-01, -9.029610e-01, -9.237818e-01, -9.423224e-01, -9.584707e-01, -9.721190e-01, -9.831646e-01, -9.915106e-01, -9.970664e-01, -9.997487e-01, -9.994819e-01, 
    -9.961988e-01, -9.898418e-01, -9.803629e-01, -9.677245e-01, -9.519005e-01, -9.328764e-01, -9.106500e-01, -8.852320e-01, -8.566466e-01, -8.249317e-01, -7.901394e-01, -7.523365e-01, -7.116044e-01, -6.680397e-01, -6.217540e-01, -5.728744e-01, 
    -5.215427e-01, -4.679162e-01, -4.121667e-01, -3.544809e-01, -2.950593e-01, -2.341163e-01, -1.718792e-01, -1.085878e-01, -4.449344e-02, 2.014195e-02, 8.504666e-02, 1.499405e-01, 2.145357e-01, 2.785386e-01, 3.416508e-01, 4.035704e-01, 
    4.639941e-01, 5.226181e-01, 5.791405e-01, 6.332623e-01, 6.846898e-01, 7.331362e-01, 7.783231e-01, 8.199827e-01, 8.578599e-01, 8.917133e-01, 9.213181e-01, 9.464667e-01, 9.669717e-01, 9.826664e-01, 9.934072e-01, 9.990747e-01, 
    9.995753e-01, 9.948422e-01, 9.848368e-01, 9.695497e-01, 9.490014e-01, 9.232428e-01, 8.923560e-01, 8.564546e-01, 8.156836e-01, 7.702190e-01, 7.202682e-01, 6.660690e-01, 6.078885e-01, 5.460229e-01, 4.807954e-01, 4.125557e-01, 
    3.416772e-01, 2.685562e-01, 1.936091e-01, 1.172703e-01, 3.998997e-02, -3.776908e-02, -1.155338e-01, -1.928240e-01, -2.691554e-01, -3.440430e-01, -4.170040e-01, -4.875614e-01, -5.552473e-01, -6.196062e-01, -6.801984e-01, -7.366034e-01, 
    -7.884232e-01, -8.352851e-01, -8.768453e-01, -9.127912e-01, -9.428448e-01, -9.667644e-01, -9.843479e-01, -9.954339e-01, -9.999040e-01, -9.976844e-01, -9.887466e-01, -9.731088e-01, -9.508362e-01, -9.220411e-01, -8.868829e-01, -8.455675e-01, 
    -7.983463e-01, -7.455152e-01, -6.874129e-01, -6.244186e-01, -5.569501e-01, -4.854611e-01, -4.104379e-01, -3.323966e-01, -2.518792e-01, -1.694498e-01, -8.569114e-02, -1.199563e-03, 8.341874e-02, 1.675525e-01, 2.505897e-01, 3.319228e-01, 
    4.109526e-01, 4.870937e-01, 5.597784e-01, 6.284616e-01, 6.926252e-01, 7.517820e-01, 8.054799e-01, 8.533056e-01, 8.948879e-01, 9.299013e-01, 9.580681e-01, 9.791614e-01, 9.930070e-01, 9.994851e-01, 9.985312e-01, 9.901373e-01, 
    9.743520e-01, 9.512803e-01, 9.210833e-01, 8.839769e-01, 8.402302e-01, 7.901640e-01, 7.341480e-01, 6.725983e-01, 6.059742e-01, 5.347746e-01, 4.595342e-01, 3.808195e-01, 2.992241e-01, 2.153643e-01, 1.298743e-01, 4.340098e-02, 
    -4.340098e-02, -1.298743e-01, -2.153643e-01, -2.992241e-01, -3.808195e-01, -4.595342e-01, -5.347746e-01, -6.059742e-01, -6.725983e-01, -7.341480e-01, -7.901640e-01, -8.402302e-01, -8.839769e-01, -9.210833e-01, -9.512803e-01, -9.743520e-01, 
    -9.901373e-01, -9.985312e-01, -9.994851e-01, -9.930070e-01, -9.791614e-01, -9.580681e-01, -9.299013e-01, -8.948879e-01, -8.533056e-01, -8.054799e-01, -7.517820e-01, -6.926252e-01, -6.284616e-01, -5.597784e-01, -4.870937e-01, -4.109526e-01, 
    -3.319228e-01, -2.505897e-01, -1.675525e-01, -8.341874e-02, 1.199563e-03, 8.569114e-02, 1.694498e-01, 2.518792e-01, 3.323966e-01, 4.104379e-01, 4.854611e-01, 5.569501e-01, 6.244186e-01, 6.874129e-01, 7.455152e-01, 7.983463e-01, 
    8.455675e-01, 8.868829e-01, 9.220411e-01, 9.508362e-01, 9.731088e-01, 9.887466e-01, 9.976844e-01, 9.999040e-01, 9.954339e-01, 9.843479e-01, 9.667644e-01, 9.428448e-01, 9.127912e-01, 8.768453e-01, 8.352851e-01, 7.884232e-01, 
    7.366034e-01, 6.801984e-01, 6.196062e-01, 5.552473e-01, 4.875614e-01, 4.170040e-01, 3.440430e-01, 2.691554e-01, 1.928240e-01, 1.155338e-01, 3.776908e-02, -3.998997e-02, -1.172703e-01, -1.936091e-01, -2.685562e-01, -3.416772e-01, 
    -4.125557e-01, -4.807954e-01, -5.460229e-01, -6.078885e-01, -6.660690e-01, -7.202682e-01, -7.702190e-01, -8.156836e-01, -8.564546e-01, -8.923560e-01, -9.232428e-01, -9.490014e-01, -9.695497e-01, -9.848368e-01, -9.948422e-01, -9.995753e-01, 
    -9.990747e-01, -9.934072e-01, -9.826664e-01, -9.669717e-01, -9.464667e-01, -9.213181e-01, -8.917133e-01, -8.578599e-01, -8.199827e-01, -7.783231e-01, -7.331362e-01, -6.846898e-01, -6.332623e-01, -5.791405e-01, -5.226181e-01, -4.639941e-01, 
    -4.035704e-01, -3.416508e-01, -2.785386e-01, -2.145357e-01, -1.499405e-01, -8.504666e-02, -2.014195e-02, 4.449344e-02, 1.085878e-01, 1.718792e-01, 2.341163e-01, 2.950593e-01, 3.544809e-01, 4.121667e-01, 4.679162e-01, 5.215427e-01, 
    5.728744e-01, 6.217540e-01, 6.680397e-01, 7.116044e-01, 7.523365e-01, 7.901394e-01, 8.249317e-01, 8.566466e-01, 8.852320e-01, 9.106500e-01, 9.328764e-01, 9.519005e-01, 9.677245e-01, 9.803629e-01, 9.898418e-01, 9.961988e-01, 
    9.994819e-01, 9.997487e-01, 9.970664e-01, 9.915106e-01, 9.831646e-01, 9.721190e-01, 9.584707e-01, 9.423224e-01, 9.237818e-01, 9.029610e-01, 8.799760e-01, 8.549455e-01, 8.279909e-01, 7.992354e-01, 7.688033e-01, 7.368199e-01, 
    7.034103e-01, 6.686995e-01, 6.328115e-01, 5.958693e-01, 5.579939e-01, 5.193045e-01, 4.799176e-01, 4.399472e-01, 3.995040e-01, 3.586957e-01, 3.176261e-01, 2.763955e-01, 2.351000e-01, 1.938318e-01, 1.526788e-01, 1.117245e-01, 
    7.104823e-02, 3.072451e-02, -9.176440e-03, -4.858895e-02, -8.745188e-02, -1.257086e-01, -1.633068e-01, -2.001988e-01, -2.363410e-01, -2.716942e-01, -3.062233e-01, -3.398972e-01, -3.726888e-01, -4.045746e-01, -4.355352e-01, -4.655542e-01, 
    -4.946190e-01, -5.227201e-01, -5.498511e-01, -5.760084e-01, -6.011916e-01, -6.254025e-01, -6.486455e-01, -6.709276e-01, -6.922576e-01, -7.126466e-01, -7.321075e-01, -7.506548e-01, -7.683047e-01, -7.850750e-01, -8.009845e-01, -8.160534e-01, 
    -8.303029e-01, -8.437550e-01, -8.564329e-01, -8.683600e-01, -8.795606e-01, -8.900593e-01, -8.998813e-01, -9.090519e-01, -9.175966e-01, -9.255412e-01, -9.329113e-01, -9.397325e-01, -9.460305e-01, -9.518305e-01, -9.571577e-01, -9.620368e-01, 
    -9.664923e-01, -9.705483e-01, -9.742282e-01, -9.775553e-01, -9.805519e-01, -9.832401e-01, -9.856413e-01, -9.877762e-01, -9.896648e-01, -9.913266e-01, -9.927801e-01, -9.940434e-01, -9.951336e-01, -9.960671e-01, -9.968597e-01, -9.975262e-01, 
    -9.980806e-01, -9.985363e-01, -9.989056e-01, -9.992003e-01, -9.994310e-01, -9.996079e-01, -9.997399e-01, -9.998354e-01, -9.999018e-01, -9.999458e-01, -9.999730e-01, -9.999884e-01, -9.999960e-01, -9.999991e-01, -9.999999e-01, -1.000000e+00, 
    -1.000000e+00, -9.999997e-01, -9.999980e-01, -9.999930e-01, -9.999819e-01, -9.999611e-01, -9.999262e-01, -9.998718e-01, -9.997917e-01, -9.996789e-01, -9.995256e-01, -9.993230e-01, -9.990616e-01, -9.987310e-01, -9.983200e-01, -9.978166e-01, 
    -9.972078e-01, -9.964801e-01, -9.956189e-01, -9.946091e-01, -9.934344e-01, -9.920782e-01, -9.905229e-01, -9.887501e-01, -9.867408e-01, -9.844753e-01, -9.819332e-01, -9.790935e-01, -9.759344e-01, -9.724338e-01, -9.685688e-01, -9.643160e-01, 
    -9.596517e-01, -9.545517e-01, -9.489912e-01, -9.429453e-01, -9.363889e-01, -9.292964e-01, -9.216423e-01, -9.134009e-01, -9.045464e-01, -8.950533e-01, -8.848961e-01, -8.740495e-01, -8.624888e-01, -8.501893e-01, -8.371272e-01, -8.232792e-01, 
    -8.086227e-01, -7.931361e-01, -7.767987e-01, -7.595909e-01, -7.414944e-01, -7.224922e-01, -7.025690e-01, -6.817110e-01, -6.599062e-01, -6.371446e-01, -6.134183e-01, -5.887217e-01, -5.630516e-01, -5.364071e-01, -5.087905e-01, -4.802066e-01, 
    -4.506632e-01, -4.201717e-01, -3.887462e-01, -3.564048e-01, -3.231689e-01, -2.890638e-01, -2.541185e-01, -2.183662e-01, -1.818439e-01, -1.445931e-01, -1.066594e-01, -6.809276e-02, -2.894768e-02, 1.071698e-02, 5.083780e-02, 9.134687e-02, 
    1.321718e-01, 1.732355e-01, 2.144569e-01, 2.557500e-01, 2.970248e-01, 3.381872e-01, 3.791389e-01, 4.197779e-01, 4.599983e-01, 4.996910e-01, 5.387436e-01, 5.770408e-01, 6.144646e-01, 6.508949e-01, 6.862097e-01, 7.202855e-01, 
    7.529977e-01, 7.842211e-01, 8.138305e-01, 8.417010e-01, 8.677089e-01, 8.917317e-01, 9.136493e-01, 9.333442e-01, 9.507024e-01, 9.656139e-01, 9.779734e-01, 9.876809e-01, 9.946427e-01, 9.987717e-01, 9.999883e-01, 9.982212e-01, 
    9.934078e-01, 9.854952e-01, 9.744405e-01, 9.602119e-01, 9.427889e-01, 9.221631e-01, 8.983387e-01, 8.713332e-01, 8.411774e-01, 8.079164e-01, 7.716095e-01, 7.323309e-01, 6.901695e-01, 6.452294e-01, 5.976299e-01, 5.475056e-01, 
    4.950059e-01, 4.402955e-01, 3.835537e-01, 3.249740e-01, 2.647641e-01, 2.031448e-01, 1.403499e-01, 7.662487e-02, 1.222661e-02, -5.257797e-02, -1.175127e-01, -1.822936e-01, -2.466298e-01, -3.102249e-01, -3.727786e-01, -4.339883e-01, 
    -4.935500e-01, -5.511608e-01, -6.065199e-01, -6.593310e-01, -7.093033e-01, -7.561541e-01, -7.996101e-01, -8.394095e-01, -8.753040e-01, -9.070602e-01, -9.344616e-01, -9.573106e-01, -9.754299e-01, -9.886641e-01, -9.968816e-01, -9.999757e-01, 
    -9.978660e-01, -9.904998e-01, -9.778529e-01, -9.599307e-01, -9.367689e-01, -9.084340e-01, -8.750238e-01, -8.366675e-01, -7.935257e-01, -7.457903e-01, -6.936836e-01, -6.374585e-01, -5.773967e-01, -5.138081e-01, -4.470293e-01, -3.774220e-01, 
    -3.053715e-01, -2.312842e-01, -1.555859e-01, -7.871920e-02, -1.140920e-03, 7.668060e-02, 1.542684e-01, 2.311399e-01, 3.068099e-01, 3.807943e-01, 4.526126e-01, 5.217921e-01, 5.878705e-01, 6.503999e-01, 7.089498e-01, 7.631104e-01, 
    8.124961e-01, 8.567482e-01, 8.955382e-01, 9.285705e-01, 9.555849e-01, 9.763593e-01, 9.907115e-01, 9.985016e-01, 9.996333e-01, 9.940552e-01, 9.817622e-01, 9.627960e-01, 9.372451e-01, 9.052456e-01, 8.669802e-01, 8.226776e-01, 
    7.726118e-01, 7.171002e-01, 6.565020e-01, 5.912160e-01, 5.216785e-01, 4.483597e-01, 3.717612e-01, 2.924126e-01, 2.108675e-01, 1.276997e-01, 4.349925e-02, -4.113189e-02, -1.255845e-01, -2.092463e-01, -2.915070e-01, -3.717626e-01, 
    -4.494202e-01, -5.239027e-01, -5.946532e-01, -6.611396e-01, -7.228585e-01, -7.793401e-01, -8.301510e-01, -8.748986e-01, -9.132342e-01, -9.448559e-01, -9.695110e-01, -9.869989e-01, -9.971723e-01, -9.999390e-01, -9.952625e-01, -9.831633e-01, 
    -9.637182e-01, -9.370603e-01, -9.033782e-01, -8.629147e-01, -8.159650e-01, -7.628747e-01, -7.040371e-01, -6.398902e-01, -5.709137e-01, -4.976248e-01, -4.205746e-01, -3.403438e-01, -2.575380e-01, -1.727829e-01, -8.671976e-02, -3.462607e-15
};

#endif // WAVETABLE_FOLDSINE_16_H