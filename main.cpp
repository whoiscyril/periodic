#include "atomic_structure.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <cmath>
#include <Simple_Math.h>

int main(int argc, char const *argv[])
{
    std::string filename = "input.gin";
    std::vector<Atom> atoms;
    std::ifstream input(filename);

    if (input.is_open()) {
        std::string line;
        std::string match = "frac";
        bool foundMatch = false; // Flag to track whether the match has been found
        int lineCount = 0; // Counter variable for the lines read

        while (std::getline(input, line)) {
            if (line == match) {
                foundMatch = true;
                continue; // Skip the line containing the match
            }

            if (foundMatch) {
                Atom atom;
                std::stringstream ss(line);

                ss >> atom.label >> atom.type >> atom.x >> atom.y >> atom.z; // Extract the coordinates

                atoms.push_back(atom); // Add the Atom to the vector

                lineCount++;

                if (lineCount == 10) {
                    break; // Exit the loop after reading 12 lines
                }
            }
        }

        input.close(); // Close the file
    } else {
        std::cout << "Failed to open the file." << std::endl;
        return 1; // Exit the program with an error code
    }

 
    //Create PBC

  	Atom defect_centre;
    std::vector<double> lattice_parameter(3);

    defect_centre.label = "O";
    defect_centre.x = 0.5;
    defect_centre.y = 1;
    defect_centre.z = 1;

    lattice_parameter[0] = 4.123773;
    lattice_parameter[1] = 4.123773;
    lattice_parameter[2] = 4.123773;




    double size = 25;

    int bound = std::ceil(size / lattice_parameter[0]);

    std::vector<Atom> periodic_atoms;

for (int i = -bound; i <= bound; i++)
{
    for (int j = -bound; j <= bound; j++)
    {
        for (int k = -bound; k <= bound; k++)  // Corrected line
        {
            for (auto& atom : atoms)
            {
                Atom periodic_atom = atom;
                periodic_atom.x += i;
                periodic_atom.y += j;  // Corrected line
                periodic_atom.z += k;  // Corrected line
                periodic_atoms.push_back(periodic_atom);
            }
        
        }
    }
}

// std::cout << periodic_atoms.size() << std::endl;
// std::cout << bound << std::endl;

// for (const auto& atom:atoms)
// {
//     std::cout << atom.label << atom.type << atom.x << atom.y << atom.z << std::endl;
// }


    //Convert frac to cart

    for (auto& atom : periodic_atoms) {
        atom.x *= lattice_parameter[0];
        atom.y *= lattice_parameter[1];
        atom.z *= lattice_parameter[2];
    }
    defect_centre.x *= lattice_parameter[0];
    defect_centre.y *= lattice_parameter[1];
    defect_centre.z *= lattice_parameter[2];

    // for (const auto& atom : periodic_atoms) {
    //     std::cout << "Atom label: " << atom.label << std::endl;
    //     std::cout << "Updated coordinates (x, y, z): " << atom.x << ", " << atom.y << ", " << atom.z << std::endl;
    // }

    //Calculate unique distances

    std::vector<double> dist_ba;
    std::vector<double> dist_sn;

for (auto& atom : periodic_atoms)
{

    double distance = get_distance(atom, defect_centre);
    if (atom.label =="Ba")
    {
        dist_ba.push_back(distance);
    }
    else if (atom.label=="Sn")
    {
        dist_sn.push_back(distance);
    }

}
std::sort(dist_ba.begin(), dist_ba.end());
std::sort(dist_sn.begin(), dist_sn.end());

dist_ba.erase(std::unique(dist_ba.begin(), dist_ba.end(),
                 [](double l, double r) { return std::abs(l - r) < 0.01; }),
               dist_ba.end());
dist_sn.erase(std::unique(dist_sn.begin(), dist_sn.end(),
                 [](double l, double r) { return std::abs(l - r) < 0.01; }),
               dist_sn.end());
std::vector<double> mid_ba;
    for (std::size_t i = 1; i < dist_ba.size() - 1; i++) {
        double midpoint = (dist_ba[i] + dist_ba[i + 1]) / 2.0;
        mid_ba.push_back(midpoint);
    }
    std::vector<double> mid_sn;
    for (std::size_t i = 1; i < dist_sn.size() - 1; i++) {
        double midpoint = (dist_sn[i] + dist_sn[i + 1]) / 2.0;
        mid_sn.push_back(midpoint);
    }


for (const auto& elem : mid_ba)
{
	std::cout << elem << std::endl;
}


    return 0;
}
