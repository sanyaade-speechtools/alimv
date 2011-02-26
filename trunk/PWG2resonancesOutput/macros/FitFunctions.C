void FitFunctions()
{

   Printf("Adding BreitWignerFun");
   Printf("Adding BreitWignerGausFun");

}

Double_t BreitWignerFun(Double_t *x, Double_t *par)
{
   return par[0] / ((x[0] - par[1]) * (x[0] - par[1]) + ((par[2] + 0.00688) * (par[2] + 0.00688)) / 4.);
}
Double_t brewigGaufun(Double_t *x, Double_t *par)
{

   //Fit parameters:
   //par[0]=Width (#Gamma) parameter of Breit-Wigner density
   //par[1]=Mean value (#mu) parameter of Breit-Wigner density and of convoluted Gaussian function
   //par[2]=Total area (integral -inf to inf, normalization constant)
   //par[3]=Width (#sigma) of convoluted Gaussian function
   //

   // Numeric constant
   const Double_t invsq2pi = 1. / TMath::Sqrt(2.*TMath::Pi());

   // Control constants
   Double_t np = 250;      // number of convolution steps
   Double_t sc =   5;      // convolution extends to +-sc Gaussian sigmas

   // Variables
   Double_t xx;
   Double_t fland;
   Double_t sum = 0;
   Double_t xlow, xupp;
   Double_t step;

   // Range of convolution integral
   xlow = x[0] - sc * par[3];
   xupp = x[0] + sc * par[3];

   step = (xupp - xlow) / np;

   // Convolution integral of Breit-Wigner and Gaussian by sum
   for (Int_t i = 1; i <= ((Int_t)(np / 2)); i++) {
      xx = xlow + ((Float_t)i - .5) * step;
      fland = TMath::BreitWigner(xx, par[1], par[0]) / par[0];
      sum += fland * TMath::Gaus(x[0], xx, par[3]);

      xx = xupp - ((Float_t)i - .5) * step;
      fland = TMath::BreitWigner(xx, par[1], par[0]) / par[0];
      sum += fland * TMath::Gaus(x[0], xx, par[3]);
   }

   return (par[2] * step * sum * invsq2pi / par[3]);

}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

TF1 *brewigGaufit(TH1F *his, Double_t *fitrange, Double_t *startvalues, Double_t *parlimitslo, Double_t *parlimitshi, Double_t *fitparams, Double_t *fiterrors, Double_t *ChiSqr, Int_t *NDF)
{
   // Once again, here are the Breit-Wigner * Gaussian parameters:
   //par[0]=Width (#Gamma) parameter of Breit-Wigner density
   //par[1]=Mean value (#mu) parameter of Breit-Wigner density and of convoluted Gaussian function
   //par[2]=Total area (integral -inf to inf, normalization constant)
   //par[3]=Width (#sigma) of convoluted Gaussian function
   //
   // Variables for brewigGaufit call:
   //   his             histogram to fit
   //   fitrange[2]     lo and hi boundaries of fit range
   //   startvalues[4]  reasonable start values for the fit
   //   parlimitslo[4]  lower parameter limits
   //   parlimitshi[4]  upper parameter limits
   //   fitparams[4]    returns the final fit parameters
   //   fiterrors[4]    returns the final fit errors
   //   ChiSqr          returns the chi square
   //   NDF             returns ndf

   Int_t i;
   Char_t FunName[100];

   Printf("Fitfcn_%s", his->GetName());

   TF1 *ffitold = (TF1*)gROOT->GetListOfFunctions()->FindObject(FunName);
   if (ffitold) delete ffitold;

   TF1 *ffit = new TF1(FunName, brewigGaufun, fitrange[0], fitrange[1], 4);
   ffit->SetParameters(startvalues);
   ffit->SetParNames("Width", "Mean", "Area", "Sigma");

   for (i = 0; i < 4; i++) {
      ffit->SetParLimits(i, parlimitslo[i], parlimitshi[i]);
   }

   his->Fit(FunName, "RB0");  // fit within specified range, use ParLimits, do not plot

   ffit->GetParameters(fitparams);    // obtain fit parameters
   for (i = 0; i < 4; i++) {
      fiterrors[i] = ffit->GetParError(i);     // obtain fit parameter errors
   }

   ChiSqr[0] = ffit->GetChisquare();  // obtain chi^2
   NDF[0] = ffit->GetNDF();           // obtain ndf

   return (ffit);              // return fit function

}

Int_t brewigGaupro(Double_t *params, Double_t &maxx, Double_t &FWHM)
{

   // Seaches for the location (x value) at the maximum of the
   // Breit-Wigner-Gaussian convolute and its full width at half-maximum.
   //
   // The search is probably not very efficient, but it's a first try.

   Double_t p, x, fx, fxr, fxl;
   Double_t step;
   Double_t l, lold;
   Int_t i = 0;
   Int_t MAXCALLS = 10000;


   // Search for maximum
   p = params[1] - 0.1 * params[0];
   step = 0.05 * params[0];
   lold = -2.0;
   l    = -1.0;

   while ((l != lold) && (i < MAXCALLS)) {
      i++;
      lold = l;
      x = p + step;
      l = brewigGaufun(&x, params);
      if (l < lold) step = -step / 10;
      p += step;
   }

   if (i == MAXCALLS) return (-1);

   maxx = x;
   fx = l / 2;


   // Search for right x location of fx
   p = maxx + params[0];
   step = params[0];
   lold = -2.0;
   l    = -1e300;
   i    = 0;

   while ((l != lold) && (i < MAXCALLS)) {
      i++;
      lold = l;
      x = p + step;
      l = TMath::Abs(brewigGaufun(&x, params) - fx);
      if (l > lold) step = -step / 10;
      p += step;
   }

   if (i == MAXCALLS) return (-2);
   fxr = x;


   // Search for left x location of fx
   p = maxx - 0.5 * params[0];
   step = -params[0];
   lold = -2.0;
   l    = -1e300;
   i    = 0;

   while ((l != lold) && (i < MAXCALLS)) {
      i++;
      lold = l;
      x = p + step;
      l = TMath::Abs(brewigGaufun(&x, params) - fx);
      if (l > lold) step = -step / 10;
      p += step;
   }

   if (i == MAXCALLS) return (-3);
   fxl = x;


   FWHM = fxr - fxl;
   return (0);
}
