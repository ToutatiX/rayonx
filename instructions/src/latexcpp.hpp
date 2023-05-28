#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdlib>
#include <memory>

#include "classes.hpp"

std::ofstream file;     

namespace Latex {
    namespace {
        std::string path_ = DOC_NAME;

        /**
         * @brief Init the document
         * @param path The path of the document file
        */
        void init() {
            file.open("../" + path_ + ".tex");

            file << "\\makeatletter\n\t\\def\\input@path{{./src/tex/}}\n\\makeatother\n";
            file << "\\documentclass[9pt]{developercv}\n\n";
            file << "\\begin{document}\n\\setlength{\\tabcolsep}{16pt}\n\\renewcommand{\\arraystretch}{1.5}\n\\includepdf[pages=-]{./src/tex/firstpage.pdf}\n";
        }

        struct NamespaceInitializer {
            NamespaceInitializer() {
                init();
            }
        };
        static NamespaceInitializer initializer;
    }

    /**
     * @brief Close the document file and convert the TEX file to PDF
    */
    void close() {
        file << "\\end{document}" << std::endl;
        file.close();

        std::string command = "pdflatex -interaction=batchmode -output-directory=../ ../" + path_ + ".tex";
        system(command.c_str());
    }

    /**
     * @brief Add a new element in the TEX file
    */
    template <typename T>
    void add(T element) {
        element.print(file);
        file << "\n\n";
    }
};