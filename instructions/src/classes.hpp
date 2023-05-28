/**
* @class Base
* @brief Abstract base class for all the LaTeX elements.
* It defines the interface that all the elements should implement, by providing a pure virtual function print, which takes an std::ostream as argument and writes the LaTeX code corresponding to the element.
* It also provides a default constructor and a virtual destructor to allow polymorphism.
*/
class Base {
    public:
        Base() = default;
        virtual ~Base() {}
        virtual void print(std::ostream& file) const = 0;
};


/**
* @class Concatenate
* @brief Concatenate class represents a sequence of LaTeX elements that should be printed one after another.
* It stores a vector of pointers to Base objects, and implements the print function to call the print function of each element in the sequence.
*/
class Concatenate : public Base {
    private:
        std::vector<Base*> elements_;

    public:
        Concatenate(std::vector<Base*> elements) 
        : elements_{} {
            for (Base* element : elements) {
                elements_.push_back(element);
            }
        }

        void print(std::ostream& file) const override {
            for (const auto& elem : elements_) {
                elem->print(file);
            }
        }
};

/**
* @class Paragraph
* @brief Paragraph class represents a paragraph of text.
* It stores a string of text, and implements the print function to write the text directly to the output stream.
*/
class Paragraph : public Base {
    private:
        std::string text_;
        
    public:
        Paragraph(std::string text) : text_(text) {
        }

        void print(std::ostream& file) const override {
            file << text_;
        }

};

/**
* @class Title
* @brief Title class represents a title element.
* It stores a string of text, and implements the print function to write the LaTeX code for a centered and huge text element.
*/
class Title{
    private:
        std::string text_;
        
    public:
        Title(std::string text) : text_(text) {
        }

        void print(std::ostream& file) const {
            file << "\\vspace{5mm}\n";
            file << "\\begin{center}\n";
            file << "\t\\huge \\textbf{" << text_ << "}\n";
            file << "\\end{center}";
        }
};

/**
* @class Large title
* @brief Title class represents a large title element.
* It stores a string of text, and implements the print function to write the LaTeX code for a centered and huge text element.
*/
class LargeTitle {
    private:
        std::string text_;
        
    public:
        LargeTitle(std::string text) : text_(text) {
        }

        void print(std::ostream& file) const {
            file << "\\vspace{5mm}\n";
            file << "\\begin{center}\n";
            file << "\t{\\HUGE\\textbf{\\MakeUppercase{"<< text_ <<"}}}\n";
            file << "\\end{center}";
        }
};

/**
* @class SubTitle
* @brief SubTitle class represents a subtitle element.
* It stores a string of text, and implements the print function to write the LaTeX code for a subtitle element using the cvsect macro.
*/
class SubTitle {
    private:
        std::string text_;
        
    public:
        SubTitle(std::string text) : text_(text) {
        }

        void print(std::ostream& file) const {
            file << "\\cvsect{" << text_ << "}";
        }
};

/**
* @class Page break
* @brief Go to a new page.
* It implements the print function to write the LaTeX code to go to a new page.
*/
class PageBreak {
    public:
        PageBreak() {}

     void print(std::ostream& file) const {
            file << "\\pagebreak";
        }
};

/**
* @class Vertical Spacing
* @brief Create a vertical spacing of the given size in mm
* It implements the print function to write the LaTeX code for a vspace.
*/
class VSpace {
    private:
        int space_;

    public:
        VSpace(int space = 5) : space_(space) {}

    void print(std::ostream& file) const {
        file << "\\vspace{" << std::to_string(space_) << " mm}";
    }
}; 

/**
* @class Horizontal Spacing
* @brief Create a horizontal spacing of the given size in mm
* It implements the print function to write the LaTeX code for a hspace.
*/
class HSpace : public Base {
    private:
        int space_;

    public:
        HSpace(int space = 5) : space_(space) {}

    void print(std::ostream& file) const override {
        file << "\\hspace{" << space_ << "mm}";
    }
}; 

/**
* @class Hyperlink
* @brief Hyperlink class represents a hyperlink element.
* It stores a string of text and a string with the URL to be linked, and implements the print function to write the LaTeX code for an underlined hyperlink.
*/
class Hyperlink : public Base {
    private:
        std::string text_;
        std::string url_;

    public:
        Hyperlink(const std::string& text, const std::string& url) 
            : text_(text), url_(url) {}

        void print(std::ostream& file) const override {
            file << "\\underline{\\href{" << url_ << "}{" << text_ << "}}";
        }
};

/**
* @class Italic
* @brief Italic class represents an italicized text element.
* It stores a string of text, and implements the print function to write the LaTeX code for an italicized text element.
*/
class Italic : public Base {
    private:
        std::string text_;

    public:
        Italic(const std::string& text) 
            : text_(text) {}

        void print(std::ostream& file) const override {
            file << "\\textit{" << text_ << "}";
        }
};

/**
* @class Enumerate
* @brief Enumerate class represents an enumerated list of items.
* It stores a vector of strings, and implements the print function to write the LaTeX code for an enumerated list of items.
*/
class Enumerate {
    private:
        std::vector<Base*> items_;

    public:
        Enumerate(const std::vector<Base*>& items) 
        : items_{} {
            for (Base* item : items) {
                items_.push_back(item);
            }
        }

        Enumerate(const std::vector<std::string>& items) 
         : items_{} {
            for (std::string str : items) {
                items_.push_back(new Paragraph(str));
            }
        }

        void print(std::ostream& file) const {
            file << "\\begin{enumerate}\n";
            for (const auto& item : items_) {
                file << "\t\\item ";
                item->print(file);
                file << "\n";
            }
            file << "\\end{enumerate}\n";
        }
};

/**
* @class Itemize
* @brief Itemize class represents an itemized list of items.
* It stores a vector of strings, and implements the print function to write the LaTeX code for an itemized list of items.
*/
class Itemize {
    private:
        std::vector<Base*> items_;

    public:
        Itemize(const std::vector<Base*>& items)
        : items_{} {
            for (Base* item : items) {
                items_.push_back(item);
            }
        }

        Itemize(const std::vector<std::string>& items) 
        : items_{} {
            for (std::string str : items) {
                items_.push_back(new Paragraph(str));
            }
        }

        void print(std::ostream& file) const {
            file << "\\begin{itemize}\n";
            for (const auto& item : items_) {
                file << "\t\\item ";
                item->print(file);
                file << "\n";
            }
            file << "\\end{itemize}\n";
        }
};



/**
* @class Bold
* @brief Bold class represents a bold text element.
* It stores a string of text, and implements the print function to write the LaTeX code for a bold text element.
*/
class Bold : public Base {
    private:
        std::string text_;

    public:
        Bold(const std::string& text) 
            : text_(text) {}

        void print(std::ostream& file) const override {
            file << "\\textbf{" << text_ << "}";
        }
};

/**
* @class Header
* @brief Header class represents a header element with a logo, a title, a subtitle and contact information.
* It stores strings for the title, the subtitle and the logo path (fixed), and implements the print function to write the LaTeX code for a header element.
*/
class Header {
    private:
        std::string title_;
        std::string subtitle_;
        
    public:
        Header(std::string title, std::string subtitle="", std::string link="") : title_(title), subtitle_(subtitle) {
        }

        void print(std::ostream& file) const {
            file << "\\begin{center}\n"
                 << "\t\\includegraphics[scale=0.06]{./src/tex/Logo.png}\n"
                 << "\\end{center}\n"
                 << "\\begin{minipage}[t]{0.48\\textwidth}\n"
                 << "\t\\vspace{-\\baselineskip}\n"
                 << "\t\\colorbox{black}{{\\HUGE\\textcolor{white}{\\textbf{\\MakeUppercase{"<<title_<<"}}}}}\\\\\n"
                 << "\t\\vspace{6pt}\n"
                 << "\t\\huge " << subtitle_ << "\n"
                 << "\\end{minipage}\n"
                 << "\\begin{minipage}[t]{0.275\\textwidth}\n"
                 << "\t\\vspace{-\\baselineskip}\n"
                 << "\t\\hspace{1cm}\n"
                 << "\t\\end{minipage}\n"
                 << "\\begin{minipage}[t]{0.275\\textwidth}\n"
                 << "\t\\vspace{-\\baselineskip}\n"
                 << "\t\\icon{Globe}{12}{\\href{https://www.toutatix.fr}{toutatix.fr}}\\\\\n"
                 << "\t\\icon{At}{12}{\\href{mailto:info@toutatix.fr}{info@toutatix.fr}}\\\\	\n"
                 << "\\end{minipage}";
        }

};

/**
* @class Figure
* @brief Figure class represents a figure element.
* It stores strings for the path to the image file, the caption, and the label, as well as a double for the size of the figure. It provides a constructor that takes optional arguments for the caption, the label and the size. It implements the print function to write the LaTeX code for a figure element, including the optional caption and label.
*/
class Figure {
    private:
        std::string path_;
        std::string caption_;
        std::string label_;
        double size_;

    public:
        /**
         * @brief Figure constructor
         * @param path The path to the figure stored in the folder /src/graphics/
         * @param caption The figure caption
         * @param label The figure label for latex references
        */
        Figure(std::string path, double size=.5, std::string caption="", std::string label="") :
            path_(std::move(path)), caption_(std::move(caption)), label_(std::move(label)), size_(size) {}

        void print(std::ostream& file) const {
            file << "\\begin{figure}[htb]\n";
            file << "\t\\centering\n";
            file << "\t\\includegraphics[width=" << std::to_string(size_) << "\\textwidth]{./src/graphics/" << path_ << "}\n";
            if (!caption_.empty()) {file << "\t\\caption{" << caption_ << "}\n";}
            if (!label_.empty()) {file << "\t\\label{fig:" << label_ << "}\n";}
            file << "\\end{figure}";
        }
};

/**
* @class Table
* @brief Table class represents a table with specified columns and rows.
* It stores a vector of vectors of strings representing the cells, and a vector of integers representing the columns' alignment.
* The class implements the print function to write the LaTeX code for the table using the tabular environment.
*/
class Table {
    private:
        std::vector<std::vector<std::string>> data_;
        std::string caption_;
        std::string label_;

    public:
        Table(const std::vector<std::vector<std::string>>& data, const std::string& caption = "", const std::string& label = "") :
            data_(data), caption_(caption), label_(label) {}

        void print(std::ostream& file) const {
            file << "\\begin{table}[h]\n";
            file << "\\centering\n";
            file << "\\begin{tabular}{";

            bool first_cell = true;
            for (const std::string& cell : data_[0]) {
                if (first_cell) {
                    first_cell = false;
                } else {
                    file << "|";
                }
                file << "c";
            }
            file << "}\n";

            bool first_row = true;
            for (const auto& row : data_) {
                first_cell = true;

                if (!first_row) {
                    file << "\\hline\n";
                }

                for (const std::string& cell : row) {
                    if (first_cell) {
                        first_cell = false;
                    } else {
                        file << "&";
                    }
                    if (first_row) {
                        file << "\\textbf{" << cell << "}";
                    }
                    else {
                        file << cell;
                    }
                }
                first_row = false;
                file << "\\\\\n";
            }

            file << "\\end{tabular}\n";
            if (!caption_.empty()) {file << "\\caption{" << caption_ << "}\n";}
            if (!label_.empty()) {file << "\\label{tab:" << label_ << "}\n";}
            file << "\\end{table}\n";
        }
};
