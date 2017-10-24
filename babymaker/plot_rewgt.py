#!/bin/env python

import ROOT as r
import plottery_wrapper as pw
from plottery import plottery as ply

f = r.TFile("rewgt.root")

####################################################################################################

h_mu_pt_sig = f.Get("h_mu_pt_sig").Clone("Prompt")
h_mu_pt_bkg = f.Get("h_mu_pt_bkg").Clone("Fake")

h_mu_pt_sig.Scale(1. / h_mu_pt_sig.Integral())
h_mu_pt_bkg.Scale(1. / h_mu_pt_bkg.Integral())

ratio = h_mu_pt_bkg.Clone("ratio")
ratio.Divide(h_mu_pt_sig, h_mu_pt_bkg)

for i in xrange(1, ratio.GetNbinsX()+1):
    print ratio.GetBinContent(i), ratio.GetBinError(i)

pw.plot_hist_1d(h_mu_pt_sig, [h_mu_pt_bkg], [], None, {"output_name":"h_mu_pt.png", "yaxis_log":True, "legend_ncolumns":1, "legend_datalabel":"Prompt"})
pw.plot_hist_1d(None, [ratio], [], None, {"output_name":"ratio_mu_sf.png", "legend_ncolumns":1})


####################################################################################################

h_mu_pt_sig = f.Get("h_mu_rewgt_pt_sig").Clone("Prompt")
h_mu_pt_bkg = f.Get("h_mu_rewgt_pt_bkg").Clone("Fake")

h_mu_pt_sig.Scale(1. / h_mu_pt_sig.Integral())
h_mu_pt_bkg.Scale(1. / h_mu_pt_bkg.Integral())

ratio = h_mu_pt_bkg.Clone("ratio")
ratio.Divide(h_mu_pt_sig, h_mu_pt_bkg)

for i in xrange(1, ratio.GetNbinsX()+1):
    print ratio.GetBinContent(i), ratio.GetBinError(i)

pw.plot_hist_1d(h_mu_pt_bkg, [h_mu_pt_sig], [], None, {"output_name":"h_rewgt_mu_pt.png", "legend_ncolumns":1, "legend_datalabel":"Fake"})
pw.plot_hist_1d(None, [ratio], [], None, {"output_name":"ratio_rewgt_mu_sf.png", "legend_ncolumns":1})

####################################################################################################

h_mu_eta_sig = f.Get("h_mu_rewgt_eta_sig").Clone("Prometa")
h_mu_eta_bkg = f.Get("h_mu_rewgt_eta_bkg").Clone("Fake")

h_mu_eta_sig.Scale(1. / h_mu_eta_sig.Integral())
h_mu_eta_bkg.Scale(1. / h_mu_eta_bkg.Integral())

ratio = h_mu_eta_bkg.Clone("ratio")
ratio.Divide(h_mu_eta_sig, h_mu_eta_bkg)

for i in xrange(1, ratio.GetNbinsX()+1):
    print ratio.GetBinContent(i), ratio.GetBinError(i)

pw.plot_hist_1d(h_mu_eta_bkg, [h_mu_eta_sig], [], None, {"output_name":"h_rewgt_mu_eta.png", "legend_ncolumns":1, "legend_datalabel":"Fake"})
pw.plot_hist_1d(None, [ratio], [], None, {"output_name":"ratio_rewgt_mu_sf.png", "legend_ncolumns":1})

####################################################################################################

h_mu_pt_sig = f.Get("h_mu_rewgtfine_pt_sig").Clone("Prompt")
h_mu_pt_bkg = f.Get("h_mu_rewgtfine_pt_bkg").Clone("Fake")

h_mu_pt_sig.Scale(1. / h_mu_pt_sig.Integral())
h_mu_pt_bkg.Scale(1. / h_mu_pt_bkg.Integral())

ratio = h_mu_pt_bkg.Clone("ratio")
ratio.Divide(h_mu_pt_sig, h_mu_pt_bkg)

for i in xrange(1, ratio.GetNbinsX()+1):
    print ratio.GetBinContent(i), ratio.GetBinError(i)

pw.plot_hist_1d(h_mu_pt_bkg, [h_mu_pt_sig], [], None, {"output_name":"h_rewgtfine_mu_pt.png", "legend_ncolumns":1, "legend_datalabel":"Fake"})
pw.plot_hist_1d(None, [ratio], [], None, {"output_name":"ratio_rewgtfine_mu_sf.png", "legend_ncolumns":1})

####################################################################################################

h_mu_eta_sig = f.Get("h_mu_rewgtfine_eta_sig").Clone("Prometa")
h_mu_eta_bkg = f.Get("h_mu_rewgtfine_eta_bkg").Clone("Fake")

h_mu_eta_sig.Scale(1. / h_mu_eta_sig.Integral())
h_mu_eta_bkg.Scale(1. / h_mu_eta_bkg.Integral())

ratio = h_mu_eta_bkg.Clone("ratio")
ratio.Divide(h_mu_eta_sig, h_mu_eta_bkg)

for i in xrange(1, ratio.GetNbinsX()+1):
    print ratio.GetBinContent(i), ratio.GetBinError(i)

pw.plot_hist_1d(h_mu_eta_bkg, [h_mu_eta_sig], [], None, {"output_name":"h_rewgtfine_mu_eta.png", "legend_ncolumns":1, "legend_datalabel":"Fake"})
pw.plot_hist_1d(None, [ratio], [], None, {"output_name":"ratio_rewgtfine_mu_sf.png", "legend_ncolumns":1})

####################################################################################################

h_mu_sf_sig = f.Get("h_mu_sf_sig").Clone("Prompt")
h_mu_sf_bkg = f.Get("h_mu_sf_bkg").Clone("Fake")

h_mu_sf_sig.Scale(1. / h_mu_sf_sig.Integral())
h_mu_sf_bkg.Scale(1. / h_mu_sf_bkg.Integral())

ratio = h_mu_sf_bkg.Clone("ratio")
ratio.Divide(h_mu_sf_sig, h_mu_sf_bkg)

for i in xrange(1, ratio.GetNbinsX()+1):
    for j in xrange(1, ratio.GetNbinsY()+1):
        print i, j, ratio.GetBinContent(i, j)

####################################################################################################

h_mu_sf_sig_fine = f.Get("h_mu_sf_sig_fine").Clone("Prompt")
h_mu_sf_bkg_fine = f.Get("h_mu_sf_bkg_fine").Clone("Fake")

h_mu_sf_sig_fine.Scale(1. / h_mu_sf_sig_fine.Integral())
h_mu_sf_bkg_fine.Scale(1. / h_mu_sf_bkg_fine.Integral())

ratio = h_mu_sf_bkg_fine.Clone("ratio")
ratio.Divide(h_mu_sf_sig_fine, h_mu_sf_bkg_fine)

for i in xrange(1, ratio.GetNbinsX()+1):
    for j in xrange(1, ratio.GetNbinsY()+1):
        print i, j, ratio.GetBinContent(i, j)

ply.plot_hist_2d(ratio, {"output_name":"fine_sf.png"})
