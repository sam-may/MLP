#!/bin/env python

import ROOT as r
import numpy as np
from sklearn import metrics

import sys

file_name = str(sys.argv[1])
f = r.TFile("/home/users/sjmay/ML/MLP/babymaker/" + file_name + ".root")
tree = f.Get("t")

def find_nearest(array,value):
    val = np.ones_like(array)*value
    idx = (np.abs(array-val)).argmin()
    return array[idx], idx

def makeroc(xmin, xmax, name, title):
    labels = []
    weights = []
    mlp_scores = []
    bdt_scores = []
    iso_scores = []
    for event in tree:
        # For now, ignore the first half in the TTree where the MLP was trained on.
        if event.mlp < 0:
            continue;
        if event.lepton_pt < xmin or event.lepton_pt > xmax: continue
        labels.append(event.lepton_isFromW)
        mlp_scores.append(event.mlp)
        bdt_scores.append(event.bdt)
        iso_scores.append(-event.lepton_relIso03EA)
        weights.append(1)

    labels = np.array(labels)
    weights = np.array(weights)
    mlp_scores = np.array(mlp_scores)
    bdt_scores = np.array(bdt_scores)
    iso_scores = np.array(iso_scores)

    fpr, tpr, thresholds = metrics.roc_curve(labels, mlp_scores, pos_label=1)
    fpr_bdt, tpr_bdt, thresholds_bdt = metrics.roc_curve(labels, bdt_scores, pos_label=1, sample_weight=weights)
    fpr_iso, tpr_iso, thresholds_iso = metrics.roc_curve(labels, iso_scores, pos_label=1, sample_weight=weights)

    value, idx = find_nearest(thresholds_iso, -0.06)
    threshSB_bdt, idxSB_bdt = find_nearest(fpr_bdt, fpr_iso[idx])
    threshSB_mlp, idxSB_mlp = find_nearest(fpr, fpr_iso[idx])
    threshSS_bdt, idxSS_bdt = find_nearest(tpr_bdt, tpr_iso[idx])
    threshSS_mlp, idxSS_mlp = find_nearest(tpr, tpr_iso[idx])


    print('\\begin{tabular}{|c|| c| c|} \n')
    print('\\multicolumn{3}{c}{Same Signal Efficiency} \\\\ \\hline \\hline \n')
    print('Cut choice & $\\epsilon_{\\text{sig}}$ & $\\epsilon_{\\text{bkg}}$ \\\\ \\hline \n')
    print('RelIso = 0.06 & %.2f' % tpr_iso[idx] )
    print(' & %.2f' % fpr_iso[idx])
    print(' \\\\ \\hline \n')
    print('MLP 1D & %.2f' % tpr[idxSS_mlp])
    print(' & %.2f' % fpr[idxSS_mlp])
    print(' \\\\ \\hline \n')
    print('BDT & %.2f' % tpr_bdt[idxSS_bdt])
    print(' & %.2f' % fpr_bdt[idxSS_bdt])
    print(' \\\\ \\hline \n')
    print(' \\end{tabular} \\\\')
 
    print('\\begin{tabular}{|c|| c| c|} \n')
    print('\\multicolumn{3}{c}{Same Background Efficiency} \\\\ \\hline \\hline \n')
    print('Cut choice & $\\epsilon_{\\text{sig}}$ & $\\epsilon_{\\text{bkg}}$ \\\\ \\hline \n')
    print('RelIso = 0.06 & %.2f' % tpr_iso[idx] )
    print(' & %.2f' % fpr_iso[idx])
    print(' \\\\ \\hline \n')
    print('MLP 1D & %.2f' % tpr[idxSB_mlp])
    print(' & %.2f' % fpr[idxSB_mlp])
    print(' \\\\ \\hline \n')
    print('BDT & %.2f' % tpr_bdt[idxSB_bdt])
    print(' & %.2f' % fpr_bdt[idxSB_bdt])
    print(' \\\\ \\hline \n')
    print(' \\end{tabular}')

    import matplotlib as mpl
    mpl.use('Agg')
    import matplotlib.pyplot as plt
    plt.figure()
    lw = 2
    plt.plot(fpr, tpr, color='darkorange', lw=lw, label="MLP 1D")
    plt.plot(fpr_bdt, tpr_bdt, color='aqua', lw=lw, label='TMVA BDT')
    plt.plot(fpr_iso, tpr_iso, color='darkred', lw=lw, label='Rel-Iso R=0.3 EA-corr')
    plt.plot(fpr_iso[idx], tpr_iso[idx], 'r*', markersize=12, label='Current Rel-Iso Cut (0.06)') 
    plt.xscale('log')
    plt.xlim([0.001, 1.0])
    plt.ylim([0.0, 1.05])
    plt.xlabel('False Positive Rate')
    plt.ylabel('True Positive Rate')
    plt.title('ROC {}'.format(title))
    plt.legend(loc="lower right")
    plt.savefig('{}.png'.format(name))

if __name__ == "__main__":
    makeroc(0, 10000000000, "MLP_1D_rocs/" + MLP_1D_name, "inclusive in pT")
