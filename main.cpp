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

#ifdef _DEBUG
void generateData(std::string filename)
{
    std::ofstream file(filename);

    dlib::rand random_generator;

    const int num_points_in_cluster = 100;
    double x, y;
    double radius = 0.5;

    for (int i = 0; i < num_points_in_cluster; ++i)
    {
        double sign = 1;
        if (random_generator.get_random_double() < 0.5)
            sign = -1;

        x = 2 * radius * random_generator.get_random_double() - radius;
        y = sign * sqrt(radius * radius - x * x);

        file << x << ";" << y << std::endl;
    }

    radius = 10.0;
    for (long i = 0; i < num_points_in_cluster; ++i)
    {
        double sign = 1;
        if (random_generator.get_random_double() < 0.5)
            sign = -1;

        x = 2 * radius * random_generator.get_random_double() - radius;
        y = sign * sqrt(radius * radius - x * x);

        file << x << ";" << y << std::endl;
    }

    radius = 4.0;
    for (long i = 0; i < num_points_in_cluster; ++i)
    {
        double sign = 1;
        if (random_generator.get_random_double() < 0.5)
            sign = -1;
        x = 2 * radius * random_generator.get_random_double() - radius;
        y = sign * sqrt(radius * radius - x * x);

        x += 25;
        y += 25;

        file << x << ";" << y << std::endl;
    }
}
#endif

//cat kkmeans_ex.txt | ./kkmeans 3
int main(int argc, char * argv[])
{
    if (argc != 2)
    {
        return 1;
    }

#ifdef _DEBUG
    // ./kkmeans generate
    if (std::string(argv[1]) == "generate")
    {
        generateData("kkmeans_ex.txt");
        return 0;
    }
#endif

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
    test.train(samples, initialCenters);

    for (unsigned long i = 0; i < samples.size(); ++i)
    {
        std::cout << samples[i](0) << ";" << samples[i](1) << ";" << test(samples[i]) << std::endl;
    }

    return 0;
}
