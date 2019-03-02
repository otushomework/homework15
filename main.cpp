#include <cstdlib>
#include <iostream>
#include <vector>

#define _DEBUG

#include <dlib/clustering.h>
#include <dlib/rand.h>

typedef dlib::matrix<double,2,1> sample_type;
typedef dlib::radial_basis_kernel<sample_type> kernel_type;

auto split(const std::string &str, char d)
{
    std::vector<std::string> r;

    std::string::size_type start = 0;
    std::string::size_type stop = str.find_first_of(d);
    while(stop != std::string::npos)
    {
        r.push_back(str.substr(start, stop - start));

        start = stop + 1;
        stop = str.find_first_of(d, start);
    }

    r.push_back(str.substr(start));

    return r;
}

//cat kkmeans_ex.txt | kkmeans 4
int main(int argc, char * argv[])
{
    if (argc != 2)
    {
        return 1;
    }

    dlib::kcentroid<kernel_type> kc(kernel_type(0.1),0.01, 8);
    dlib::kkmeans<kernel_type> test(kc);

    std::vector<sample_type> samples;
    std::vector<sample_type> initialCenters;

    int clustersCount = atoi(argv[1]);

    for(std::string line; std::getline(std::cin, line);)
    {
        auto coords = split(line, ';');
        if ( coords.size() != 2 )
            continue;

        samples.push_back(sample_type(std::stod(coords.at(0)), std::stod(coords.at(1))));
#ifdef _DEBUG
        std::cout << "src " << coords.at(0) << " " << coords.at(1) << " " << std::stod(coords.at(0)) << " " << std::stod(coords.at(1)) << std::endl;
#endif
    }

    test.set_number_of_centers(clustersCount);
    dlib::pick_initial_centers(clustersCount, initialCenters, samples, test.get_kernel());
    test.train(samples,initialCenters);

    for (unsigned long i = 0; i < samples.size(); ++i)
    {
        std::cout << samples[i](0) << ";" << samples[i](1) << ";" << test(samples[i]) << std::endl;
    }

    return 0;
}
