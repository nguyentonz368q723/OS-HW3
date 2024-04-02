/*
Nguyen Huu Tam Ton
CS540 - HW3
04/02/2024
*/

#include <iostream>
#include <iomanip>

// I define the page size as 256 bytes (2^8) by using the PAGE_SIZE macro.
#define PAGE_SIZE 256

// I create a struct to represent a page table entry
struct PageTableEntry {
    int frameNumber; // I store the frame number mapped to this page
    bool valid; // I keep track of whether this page is loaded in memory
};

// I define the maximum number of pages and frames.
// With the NUM_PAGES and NUM_FRAMES macros, I provide the maximum number of pages and frames, respectively.
#define NUM_PAGES 1024
#define NUM_FRAMES 256

// By saving the frame number and a valid bit, I build the pageTable struct to represent an item in the page table.
PageTableEntry pageTable[NUM_PAGES];

// I label every page as invalid when I first start the page table.
void initPageTable() // All pages are first tagged as invalid in the page table by the initPageTable function.
{
    for (int i = 0; i < NUM_PAGES; i++) {
        pageTable[i].valid = false;
    }
}

// I handle a page fault by loading the page from disk into an available frame
void handlePageFault(int pageNumber) {
    // I find an available frame
    int frameNumber = -1;
    for (int i = 0; i < NUM_FRAMES; i++) {
        bool frameUsed = false;
        for (int j = 0; j < NUM_PAGES; j++) {
            if (pageTable[j].valid && pageTable[j].frameNumber == i) {
                frameUsed = true;
                break;
            }
        }
        if (!frameUsed) {
            frameNumber = i;
            break;
        }
    }

    // I map the page to the available frame
    pageTable[pageNumber].frameNumber = frameNumber;
    pageTable[pageNumber].valid = true;

    // I load the page from disk into the frame
    // (Disk operation omitted for simplicity)
}

// I translate a logical address to a physical address
int translateAddress(int logicalAddress) {
    int pageNumber = logicalAddress / PAGE_SIZE;
    int offset = logicalAddress % PAGE_SIZE;

    if (!pageTable[pageNumber].valid) {
        handlePageFault(pageNumber);
    }

    int frameNumber = pageTable[pageNumber].frameNumber;
    int physicalAddress = (frameNumber * PAGE_SIZE) + offset;

    return physicalAddress;
}

// I print the page number and offset for a given logical address
void printAddressComponents(int logicalAddress) {
    int pageNumber = logicalAddress / PAGE_SIZE;
    int offset = logicalAddress % PAGE_SIZE;

    std::cout << "Logical Address: 0x" << std::uppercase << std::hex << logicalAddress
              << " => Page Number: 0x" << std::setw(2) << std::setfill('0') << pageNumber
              << ", Offset: 0x" << std::setw(2) << std::setfill('0') << offset << std::endl;
}


int main() {
    initPageTable();

    std::cout << "Enter logical addresses (ex: 0x3A7F) (enter 'q' to quit):\n";

    std::string input;
    // I constantly read user input using a while loop until the user types 'q' or 'Q' to end the process.
    while (true) {
        std::cout << "> ";
        std::getline(std::cin, input);

        if (input == "q" || input == "Q") {
            break;
        }

        int logicalAddress;
        try {
            logicalAddress = std::stoi(input, nullptr, 16);  // The input string is converted to a hexadecimal integer
        } catch (std::invalid_argument&) {
            std::cout << "Invalid input. Please enter a hexadecimal value.\n";
            continue;
        } catch (std::out_of_range&) {
            std::cout << "Input out of range. Please try again.\n";
            continue;
        }
        // I call printAddressComponents with the transformed logical address if the conversion is successful.
        printAddressComponents(logicalAddress);
    }

    return 0;
}
