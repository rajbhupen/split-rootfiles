#include <iostream>
#include <TFile.h>
#include <TTree.h>

//void splitROOT(const char* old_filename, const char* new_filename, const char* tree_name, int N_subfiles) {
  void splitROOT(const char* old_filename, const char* new_filename, const char* tree_name) {

  // Open the input ROOT file
    TFile* input_file = TFile::Open(old_filename);
    if (!input_file || input_file->IsZombie()) {
        std::cerr << "Error: Unable to open input file " << old_filename << std::endl;
        return;
    }

    // Get the input tree
    TTree* input_tree = dynamic_cast<TTree*>(input_file->Get(tree_name));
    if (!input_tree) {
        std::cerr << "Error: Unable to get input tree " << tree_name << " from file " << old_filename << std::endl;
        input_file->Close();
        return;
    }

    // Get the number of entries in the input tree
    Long64_t NEntries = input_tree->GetEntriesFast();
    std::cout<<NEntries<<" "<<std::endl;

    // Calculate number of entries per subfile and the remaining entries for the last file
    //int n_infile = NEntries / N_subfiles;
    //int n_lastfile = NEntries - n_infile * (N_subfiles - 1);
    // Loop through the number of subfiles
    int start_counter = 0;

    int n_infile = 10000;

    int N_subfiles = NEntries / n_infile;
    int n_lastfile = NEntries - n_infile*N_subfiles;

    std::cout<<n_infile<<" "<<n_lastfile<<std::endl;

    for (int jf = 0; jf < N_subfiles; ++jf) {
        // Create a new ROOT file
        TString newfile_name = TString::Format("%s_%d.root", new_filename, jf);
	TFile* newfile = TFile::Open(newfile_name, "recreate");
	
        // Clone the input tree into the new file
        TTree* newtree = input_tree->CloneTree(0);

        // Fill the new tree with entries

        int n_entries_to_fill = (jf < (N_subfiles - 1)) ? n_infile : n_lastfile;

	std::cout<<newfile_name<<" "<<n_entries_to_fill<<std::endl;

	for (int je = 0; je < n_entries_to_fill; ++je) {
            input_tree->GetEntry(start_counter + je);
            newtree->Fill();
        }

        // Write the new tree to the new file
        newfile->Write();

        // Increment the start counter for the next subfile
        start_counter += n_entries_to_fill;

        // Close the new file
        newfile->Close();
    }

    // Close the input file
    input_file->Close();
}

int main(int argc, char* argv[]) {
    if (argc != 4) {
        std::cerr << "Usage: " << argv[0] << " old_filename new_filename tree_name" << std::endl;
        return 1;
    }

    const char* old_filename = argv[1];
    const char* new_filename = argv[2];
    const char* tree_name = argv[3];
    //int N_subfiles = std::stoi(argv[4]);

    splitROOT(old_filename, new_filename, tree_name);
    //splitROOT(old_filename, new_filename, tree_name, N_subfiles);

    return 0;
}
