#ifndef GUI_H
#define GUI_H

#include <complex>
#include <vector>

struct SignalData {
    bool running = true;
    double sample_rate = 1e6;
    std::vector<std::complex<double>> samples;
};

void run_gui(SignalData &data);

#endif
