#!/bin/env python

import ROOT as r
import numpy as np
from sklearn import metrics

f = r.TFile("/hadoop/cms/store/user/phchang/mlp/output_MLP_muon_50ktrain_50ktest_1000epoch_julianOriginal_BDTbaseline_v0.0.2_3_and_4.root")
tree = f.Get("t")


def makeroc(xmin, xmax, name, title):
    labels = []
    weights = []
    mlp_scores = []
    bdt_scores = []
    bdt2_scores = []
    bdt3_scores = []
    iso_scores = []
    for event in tree:
        # For now, ignore the first half in the TTree where the MLP was trained on.
        if event.mlp < 0:
            continue;
        if event.lepton_pt < xmin or event.lepton_pt > xmax: continue
        labels.append(event.lepton_isFromW)
        mlp_scores.append(event.mlp)
        bdt_scores.append(event.bdt)
        bdt2_scores.append(event.bdt2)
        bdt3_scores.append(event.bdt3)
        iso_scores.append(-event.lepton_relIso03EA)
        weights.append(1)

    labels = np.array(labels)
    weights = np.array(weights)
    mlp_scores = np.array(mlp_scores)
    bdt_scores = np.array(bdt_scores)
    bdt2_scores = np.array(bdt2_scores)
    bdt3_scores = np.array(bdt3_scores)
    iso_scores = np.array(iso_scores)
    fpr, tpr, thresholds = metrics.roc_curve(labels, mlp_scores, pos_label=1)
    fpr_bdt, tpr_bdt, thresholds_bdt = metrics.roc_curve(labels, bdt_scores, pos_label=1, sample_weight=weights)
    fpr_bdt2, tpr_bdt2, thresholds_bdt2 = metrics.roc_curve(labels, bdt2_scores, pos_label=1, sample_weight=weights)
    fpr_bdt3, tpr_bdt3, thresholds_bdt3 = metrics.roc_curve(labels, bdt3_scores, pos_label=1, sample_weight=weights)
    fpr_iso, tpr_iso, thresholds_iso = metrics.roc_curve(labels, iso_scores, pos_label=1, sample_weight=weights)

    import matplotlib as mpl
    mpl.use('Agg')
    import matplotlib.pyplot as plt
    plt.figure()
    lw = 2
    plt.plot(fpr, tpr, color='darkorange', lw=lw, label='Julian MLP')
    plt.plot(fpr_bdt2, tpr_bdt2, color='aqua', lw=lw, label='TMVA BDT')
    plt.plot(fpr_bdt3, tpr_bdt3, color='blue', lw=lw, label='TMVA BDT w/ IP, reliso only (no lep p4)')
    plt.plot(fpr_bdt, tpr_bdt, color='aqua', lw=lw, label='TMVA BDT large training sample', linestyle='--')
    plt.plot(fpr_iso, tpr_iso, color='darkred', lw=lw, label='Rel-Iso R=0.3 EA-corr')
    #plt.plot([0, 1], [0, 1], color='navy', lw=lw, linestyle='--')
    plt.xscale('log')
    plt.xlim([0.0, 1.0])
    plt.ylim([0.0, 1.05])
    plt.xlabel('False Positive Rate')
    plt.ylabel('True Positive Rate')
    plt.title('ROC {}'.format(title))
    plt.legend(loc="lower right")
    plt.savefig('{}.png'.format(name))

if __name__ == "__main__":
    makeroc(0, 10000000000, "plot", "inclusive in pT")
    #makeroc(0, 0.1, "plot_00_01", "0 < x < 0.1")
    #makeroc(0.1, 0.2, "plot_01_02", "0.1 < x < 0.2")
    #makeroc(0.2, 0.3, "plot_02_03", "0.2 < x < 0.3")
    #makeroc(0.3, 0.4, "plot_03_04", "0.3 < x < 0.4")
    #makeroc(0.4, 0.5, "plot_04_05", "0.4 < x < 0.5")
    #makeroc(0.5, 0.6, "plot_05_06", "0.5 < x < 0.6")
    #makeroc(0.6, 0.7, "plot_06_07", "0.6 < x < 0.7")
    #makeroc(0.7, 0.8, "plot_07_08", "0.7 < x < 0.8")
    #makeroc(0.8, 0.9, "plot_08_09", "0.8 < x < 0.9")
    #makeroc(0.9, 1.0, "plot_09_10", "0.9 < x < 1.0")
