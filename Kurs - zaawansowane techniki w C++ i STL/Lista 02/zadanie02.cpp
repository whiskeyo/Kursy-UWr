#include <iostream>
#include <fstream>
#include <memory>
#include <cstdlib>
#include <vector>

class LineWriter
{
    private:
        std::ofstream* file;
    public:
        template <typename T>
        LineWriter& save_data (T data)
        {
            *file << data << std::endl;
            return *this;
        }

        LineWriter (const std::string& path)
        {
            file = new std::ofstream(path, std::ios::out);
            if (!file->is_open()) // do tego i tak raczej nie wejdziemy, bo tworzymy plik
            {
                throw std::ofstream::failure("Failed to open specified file!!!\n");
            }
        }

        ~LineWriter()
        {
            delete file; // destruktor ofstream zamyka plik
            std::cout << "File successfully closed!\n";
        }

        LineWriter (LineWriter&& other) = delete;
        LineWriter& operator= (const LineWriter& other)  = delete;
        LineWriter& operator= (const LineWriter&& other) = delete;
};

int main()
{
    uint32_t number_of_files = 10;
    std::string path;

    std::cout << "Give the name of the file: "; std::cin >> path;
    auto writer = std::make_shared<LineWriter>(path);

    std::vector<std::shared_ptr<LineWriter>> vec_pointers;
    for (uint32_t i = 0; i < number_of_files; i++)
        vec_pointers.push_back(writer);

    for (uint32_t i = 0; i < number_of_files; i++)
        vec_pointers[i]->save_data(i);

    
    std::shared_ptr<LineWriter> shared_file0 = writer;
    shared_file0->save_data("shared_file0");

    std::shared_ptr<LineWriter> shared_file1 = shared_file0;
    shared_file1->save_data("shared_file1");

    std::shared_ptr<LineWriter> shared_file2 = writer;
    shared_file2->save_data("shared_file2");

    return 0;
}
