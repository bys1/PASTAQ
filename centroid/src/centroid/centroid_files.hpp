#ifndef CENTROID_CENTROIDFILES_HPP
#define CENTROID_CENTROIDFILES_HPP
#include <iostream>

#include "centroid/centroid.hpp"

namespace Centroid::Files::Bpks {

// The header contains the necessary information for proper interpretation of
// the serialized data.
struct Header {
    uint8_t spec_version;
    uint64_t num_peaks;
    Grid::Parameters grid_params;
};

// Read/Write the header from the given binary stream.
bool read_header(std::istream &stream, Header *header);
bool write_header(std::ostream &stream, const Header &header);

// Read/Write all peaks from the given binary stream.
bool read_peaks(std::istream &stream, Grid::Parameters *grid_parameters,
                std::vector<Centroid::Peak> *peaks);
bool write_peaks(std::ostream &stream, const Grid::Parameters &grid_parameters,
                 const std::vector<Centroid::Peak> &peaks);

}  // namespace Centroid::Files::Bpks

namespace Centroid::Files::Csv {
// Write the most significant peak values on the CSV format.
bool write_peaks(std::ostream &stream,
                 const std::vector<Centroid::Peak> &peaks);
// TODO(alex): Read peaks from csv?
// bool read_peaks(std::istream &stream, std::vector<Centroid::Peak> *peaks);
}  // namespace Centroid::Files::Csv

#endif /* CENTROID_CENTROIDFILES_HPP */