#include <algorithm>
#include <vector>
#include <numeric>
#include <functional>
#include <iostream>

// root -l -x "image.C(\"filename.root\")"

void
image(const char* filename = 0)
{
	if (filename == 0) {
		std::cout << "Usage: root -l -x \"image.C(\\\"filename.root\\\")\" " << std::endl;
		return;
	}
	TFile* file = TFile::Open(filename);

	TH2D* pos = dynamic_cast<TH2D*>(file->Get("position_object"));
	
	Int_t minybin = 10;
	Int_t maxybin = 50;
	TH1D* pos_prof = pos->ProjectionX( "position_object_projection", minybin, maxybin, "[cutg]");

	for ( Int_t i = 1; i <= pos_prof->GetNbinsX(); ++i) {
		Double_t p = pos_prof->GetBinContent(i);
		Double_t perr = pos_prof->GetBinError(i);
		pos_prof->SetBinContent( i, p / (maxybin - minybin));
		pos_prof->SetBinError( i, perr / (maxybin - minybin));
	}

	TCanvas* canvas = new TCanvas( "canvas", "Carbon-12 455 MeV/u Beam " \
		"ion radiography image");
	canvas->GetFrame()->SetBorderSize(10);
	canvas->GetFrame()->SetBorderMode(2);
	canvas->Divide( 1, 2);
	canvas->cd(1);

	pos->SetTitle("Radiographic image");
	pos->SetXTitle("Size X (mm)");
	pos->SetYTitle("Size Y (mm)");
	pos->Draw("colz");

	pos_prof->SetTitle("Projection");
	pos_prof->SetXTitle("Size X (mm)");
	pos_prof->SetYTitle("Polystyrene Equivalent Thickness (cm)");

	canvas->cd(2);
	pos_prof->Draw();
}
