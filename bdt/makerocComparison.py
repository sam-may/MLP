#!/bin/env python

import ROOT as r
import numpy as np
from sklearn import metrics

f = r.TFile("/hadoop/cms/store/user/phchang/mlp/output_MLP_muon_50ktrain_50ktest_1000epoch_julianOriginal_BDTbaseline_v0.0.2_3_and_4.root")
tree = f.Get("t")

fCompare = r.TFile("/home/users/sjmay/ML/MLP/babymaker/samTest2.root")
treeCompare = fCompare.Get("t")

def find_nearest(array,value):
    val = np.ones_like(array)*value
    idx = (np.abs(array-val)).argmin()
    return array[idx], idx

def makeroc(xmin, xmax, name, title):
    labels = []
    weights = []
    mlp_scores = []
    bdt_scores = []
    bdt2_scores = []
    bdt3_scores = []
    iso_scores = []
    mlp_comp_scores = []
    labels_comp = []
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

    for event in treeCompare:
        if event.mlp < 0:
            continue
        if event.lepton_pt < xmin or event.lepton_pt > xmax: continue
        mlp_comp_scores.append(event.mlp)
        labels_comp.append(event.lepton_isFromW)


    labels = np.array(labels)
    weights = np.array(weights)
    mlp_scores = np.array(mlp_scores)
    bdt_scores = np.array(bdt_scores)
    bdt2_scores = np.array(bdt2_scores)
    bdt3_scores = np.array(bdt3_scores)
    iso_scores = np.array(iso_scores)
    mlp_comp_scores = np.array(mlp_comp_scores)
    labels_comp = np.array(labels_comp)

    print(len(mlp_scores))
    print(len(mlp_comp_scores))

    fpr, tpr, thresholds = metrics.roc_curve(labels, mlp_scores, pos_label=1)
    fpr_bdt, tpr_bdt, thresholds_bdt = metrics.roc_curve(labels, bdt_scores, pos_label=1, sample_weight=weights)
    fpr_bdt2, tpr_bdt2, thresholds_bdt2 = metrics.roc_curve(labels, bdt2_scores, pos_label=1, sample_weight=weights)
    fpr_bdt3, tpr_bdt3, thresholds_bdt3 = metrics.roc_curve(labels, bdt3_scores, pos_label=1, sample_weight=weights)
    fpr_iso, tpr_iso, thresholds_iso = metrics.roc_curve(labels, iso_scores, pos_label=1, sample_weight=weights)
    fpr_mlp_comp, tpr_mlp_comp, thresholds_mlp_comp = metrics.roc_curve(labels_comp, mlp_comp_scores, pos_label=1)

    value, idx = find_nearest(thresholds_iso, -0.06)
    threshSB_bdt, idxSB_bdt = find_nearest(fpr_bdt, fpr_iso[idx])
    threshSB_mlp, idxSB_mlp = find_nearest(fpr, fpr_iso[idx])
    threshSS_bdt, idxSS_bdt = find_nearest(tpr_bdt, tpr_iso[idx])
    threshSS_mlp, idxSS_mlp = find_nearest(tpr, tpr_iso[idx])
    threshSB_mlp_comp, idxSB_mlp_comp = find_nearest(fpr_mlp_comp, fpr_iso[idx])
    threshSS_mlp_comp, idxSS_mlp_comp = find_nearest(tpr_mlp_comp, tpr_iso[idx])


    print('\\begin{tabular}{|c|| c| c|} \n')
    print('\\multicolumn{3}{c}{Same Signal Efficiency} \\\\ \\hline \\hline \n')
    print('Cut choice & $\\epsilon_{\\text{sig}}$ & $\\epsilon_{\\text{bkg}}$ \\\\ \\hline \n')
    print('RelIso = 0.06 & %.2f' % tpr_iso[idx] )
    print(' & %.2f' % fpr_iso[idx])
    print(' \\\\ \\hline \n')
    print('MLP & %.2f' % tpr[idxSS_mlp])
    print(' & %.2f' % fpr[idxSS_mlp])
    print(' \\\\ \\hline \n')
    print('BDT & %.2f' % tpr_bdt[idxSS_bdt])
    print(' & %.2f' % fpr_bdt[idxSS_bdt])
    print(' \\\\ \\hline \n')
    print('MLP 1D & %.2f' % tpr_mlp_comp[idxSS_mlp_comp])
    print(' & %.2f' % fpr_mlp_comp[idxSS_mlp_comp])
    print(' \\\\ \\hline \n')
    print(' \\end{tabular} \\\\')
 
    print('\\begin{tabular}{|c|| c| c|} \n')
    print('\\multicolumn{3}{c}{Same Background Efficiency} \\\\ \\hline \\hline \n')
    print('Cut choice & $\\epsilon_{\\text{sig}}$ & $\\epsilon_{\\text{bkg}}$ \\\\ \\hline \n')
    print('RelIso = 0.06 & %.2f' % tpr_iso[idx] )
    print(' & %.2f' % fpr_iso[idx])
    print(' \\\\ \\hline \n')
    print('MLP & %.2f' % tpr[idxSB_mlp])
    print(' & %.2f' % fpr[idxSB_mlp])
    print(' \\\\ \\hline \n')
    print('BDT & %.2f' % tpr_bdt[idxSB_bdt])
    print(' & %.2f' % fpr_bdt[idxSB_bdt])
    print(' \\\\ \\hline \n')
    print('MLP 1D & %.2f' % tpr_mlp_comp[idxSB_mlp_comp])
    print(' & %.2f' % fpr_mlp_comp[idxSB_mlp_comp])
    print(' \\\\ \\hline \n')
    print(' \\end{tabular}')

    import matplotlib as mpl
    mpl.use('Agg')
    import matplotlib.pyplot as plt
    plt.figure()
    lw = 2
    plt.plot(fpr, tpr, color='darkorange', lw=lw, label="Julian's MLP")
    plt.plot(fpr_bdt2, tpr_bdt2, color='aqua', lw=lw, label='TMVA BDT')
    #plt.plot(fpr_bdt3, tpr_bdt3, color='blue', lw=lw, label='TMVA BDT w/ IP, reliso only (no lep p4)')
    #plt.plot(fpr_bdt, tpr_bdt, color='aqua', lw=lw, label='TMVA BDT large training sample', linestyle='--')
    plt.plot(fpr_iso, tpr_iso, color='darkred', lw=lw, label='Rel-Iso R=0.3 EA-corr')
    plt.plot(fpr_mlp_comp, tpr_mlp_comp, color='blue', lw=lw, label='MLP 1D')
    plt.plot(fpr_iso[idx], tpr_iso[idx], 'r*', markersize=12, label='Current Rel-Iso Cut (0.06)') 
    #plt.plot([0, 1], [0, 1], color='navy', lw=lw, linestyle='--')
    plt.xscale('log')
    plt.xlim([0.001, 1.0])
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
