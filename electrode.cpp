#include "electrode.h"

#include <bits/stdc++.h> //idk if we're allowed to use this, needed for random number generation -mars

Electrode::Electrode()
{

}

float Electrode::receiveBrainwave()
{
    //using a random number for now
    float freq = (float)(rand())/(float)(rand());

    return freq;
}
