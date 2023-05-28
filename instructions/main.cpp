#define DOC_NAME  "Instructions"

#include "./src/latexcpp.hpp"

int main() {
    using namespace Latex;

    add(Header("RayonX web radio", "Instructions par ToutatiX"));

    add(Title("Bill of Materials"));

    add(Paragraph("Here is the list of all the sourced components required to build this product:"));

    add(Itemize({
        "1x Amplifier",
        "1x Speaker",
        "3x Limit Switch",
        "1x ESP32",
        "7x M3x10 HEX Screw",
        "7x M3x4 Brass inserts"
    }));

    add(Title("3D Printing"));
    add(SubTitle("Step 1"));
    add(Paragraph("The instructions are..."));
    add(Figure("test.jpeg", 0.5, "Eliott"));
    
    add(Title("Laser Cut"));
    add(SubTitle("Step 1"));
    add(Paragraph("The instructions are..."));

    add(PageBreak());
    add(VSpace(55));
    add(LargeTitle("Appendix I"));

    add(Table({
        {"Name", "Quantity", "Reference", "Link", "Supplier"},
        {"Name", "Quantity", "Reference", "Link", "Supplier"},
        {"Name", "Quantity", "Reference", "Link", "Supplier"}
    }));

    close();

    return 0;
}
