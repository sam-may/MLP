#!/bin/bash

PACKAGE=package.tar.gz


###################################################################################################
# ProjectMetis/CondorTask specific (Setting up some common environment)
###################################################################################################
#echo "To check whether it's on condor universe Vanilla or Local. The following variables are used."
#echo "If _CONDOR_SLOT is set, it's on Vanilla"
#echo "If X509_USER_PROXY is set, it's either on Vanilla or Local."
# if 
if [ "x${_CONDOR_JOB_AD}" == "x" ]; then
    :
else
    hostname
    uname -a
    date
    whoami
    pwd
    echo "ls'ing hadoop"
    ls -lh /hadoop/cms/store/user/phchang/
    echo "_CONDOR_SLOT" ${_CONDOR_SLOT}
    echo "X509_USER_PROXY" ${X509_USER_PROXY}
    echo "_CONDOR_SCRATCH_DIR"             ${_CONDOR_SCRATCH_DIR}
    echo "_CONDOR_SLOT"                    ${_CONDOR_SLOT}
    echo "CONDOR_VM"                       ${CONDOR_VM}
    echo "X509_USER_PROXY"                 ${X509_USER_PROXY}
    echo "_CONDOR_JOB_AD"                  ${_CONDOR_JOB_AD}
    echo "_CONDOR_MACHINE_AD"              ${_CONDOR_MACHINE_AD}
    echo "_CONDOR_JOB_IWD"                 ${_CONDOR_JOB_IWD}
    echo "_CONDOR_WRAPPER_ERROR_FILE"      ${_CONDOR_WRAPPER_ERROR_FILE}
    echo "CONDOR_IDS"                      ${CONDOR_IDS}
    echo "CONDOR_ID"                       ${CONDOR_ID}
    OUTPUTDIR=$1
    OUTPUTNAME=$2
    INPUTFILENAMES=$3
    IFILE=$4
    CMSSWVERSION=$5
    SCRAMARCH=$6
    if [ "x${_CONDOR_SLOT}" == "x" ]; then
        WORKDIR=/tmp/phchang_condor_local_${OUTPUTDIR//\//_}_${OUTPUTNAME}_${IFILE}
        mkdir -p ${WORKDIR}
        ls
        cp package.tar.gz ${WORKDIR}
        cd ${WORKDIR}
        ls
        echo "This is in Condor session with Universe=Local."
        echo "WORKDIR=${WORKDIR}"
        echo "pwd"
        pwd
    fi
    echo "OUTPUTDIR     : $1"
    echo "OUTPUTNAME    : $2"
    echo "INPUTFILENAMES: $3"
    echo "IFILE         : $4"
    echo "CMSSWVERSION  : $5"
    echo "SCRAMARCH     : $6"
    shift 6
    tar xvzf package.tar.gz
    if [ $? -eq 0 ]; then
        echo "Successfully untarred package."
        :
    else
        echo "Failed to untar package."
        exit
    fi
fi
###################################################################################################

#------------=================-----------------===============---------------==============--------
# Begin execution
#------------=================-----------------===============---------------==============--------

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

usage()
{
    echo "Usage: sh $(basename $0) [-p] [-g] [-c] scanchain output.root t nevents extraopt cms3_1,cms3_2.root"
    echo ""
    echo "  -g option runs in gdb"
    echo "  -c option forces recompilation"
    echo ""
    exit
}

# Command-line opts
while getopts ":gpch" OPTION; do
  case $OPTION in
    g) DEBUG=true;;
    p) PERF=true;;
    c) FORCERECOMPILE="+";;
    h) usage;;
    :) usage;;
  esac
done

# to shift away the parsed options
shift $(($OPTIND - 1))

# Parse arguments
if [ -z "$1" ]; then usage; fi
if [ -z "$2" ]; then usage; fi
if [ -z "$3" ]; then usage; fi
if [ -z "$4" ]; then usage; fi
if [ -z "$5" ]; then usage; fi
SCANCHAINNAME=$1
OUTPUTROOTNAME=$2
INPUTTTREENAME=$3
NEVENTS=$4
EXTRAOPT=$5

# Parse the input file names differently depending on whether it's condor job or local job.
if [ "x${_CONDOR_JOB_AD}" == "x" ]; then
    if [ -n "$6" ]; then INPUTFILENAMES=$6; fi
else
    # If condor jobs, touch the .so files to prevent from recompiling
    touch *.so
fi

# Setup the same root I want
source $DIR/root.sh "" &> /dev/null

# echo current settings
echo "==============================================================================="
echo "$(basename $0) $*"
echo "==============================================================================="
echo "SCANCHAINNAME  = $SCANCHAINNAME"
echo "OUTPUTROOTNAME = $OUTPUTROOTNAME"
echo "INPUTTTREENAME = $INPUTTTREENAME"
echo "NEVENTS        = $NEVENTS"
echo "EXTRAOPT       = $EXTRAOPT"
echo "INPUTFILENAMES = $INPUTFILENAMES"
echo "FORCERECOMPILE = $FORCERECOMPILE"
echo "==============================================================================="

# Run the job!
if [ "${DEBUG}" == true ]; then
    COMPILERFLAG=+g
    gdb --args root.exe -l -b -q $DIR/'run.C("'${SCANCHAINNAME}'","'${INPUTFILENAMES}'","'${INPUTTTREENAME}'","'${OUTPUTROOTNAME}'","'${NEVENTS}'", "'${COMPILERFLAG}'", "'${EXTRAOPT}'")'
else
    if [ "${PERF}" == true ]; then
        COMPILERFLAG=+g
        root.exe -l -b -q $DIR/'run.C("'${SCANCHAINNAME}'","'${INPUTFILENAMES}'","'${INPUTTTREENAME}'","'${OUTPUTROOTNAME}'","'1'", "'${COMPILERFLAG}'", "'${EXTRAOPT}'")'
        COMPILERFLAG=g
        igprof -pp -d -z -o igprof.pp.gz root.exe -l -b -q $DIR/'run.C("'${SCANCHAINNAME}'","'${INPUTFILENAMES}'","'${INPUTTTREENAME}'","'${OUTPUTROOTNAME}'","'${NEVENTS}'", "'${COMPILERFLAG}'", "'${EXTRAOPT}'")'
        igprof-analyse --sqlite -d -v -g igprof.pp.gz | sqlite3 igprof.pp.sql3 >& /dev/null
        cp igprof.pp.sql3 ~/public_html/cgi-bin/data/
        echo "http://${HOSTNAME}/~phchang/cgi-bin/igprof-navigator.py/igprof.pp/"
    else
        COMPILERFLAG=${FORCERECOMPILE}O
        if [ -z "$(ls ${SCANCHAINNAME%.*}_C.so)" ]; then
            :
        else
           touch ${SCANCHAINNAME%.*}_C.so
        fi
        echo root -l -b -q $DIR/'run.C("'${SCANCHAINNAME}'","'${INPUTFILENAMES}'","'${INPUTTTREENAME}'","'${OUTPUTROOTNAME}'","'${NEVENTS}'", "'${COMPILERFLAG}'", "'${EXTRAOPT}'")'
        time root -l -b -q $DIR/'run.C("'${SCANCHAINNAME}'","'${INPUTFILENAMES}'","'${INPUTTTREENAME}'","'${OUTPUTROOTNAME}'","'${NEVENTS}'", "'${COMPILERFLAG}'", "'${EXTRAOPT}'")'
    fi
fi

#------------=================-----------------===============---------------==============--------
# End execution
#------------=================-----------------===============---------------==============--------




###################################################################################################
# ProjectMetis/CondorTask specific (Copying files over to hadoop)
###################################################################################################
if [ "x${_CONDOR_JOB_AD}" == "x" ]; then
    :
else
    echo "==============================================================================="
    echo " Copying files to output directory"
    echo "==============================================================================="
    hostname
    uname -a
    date
    whoami
    pwd
    echo "ls'ing hadoop"
    ls -lh /hadoop/cms/store/user/phchang/
    if [[ ${OUTPUTDIR} == *"home/users/"* ]]; then
        mkdir -p ${OUTPUTDIR}
        INFILE=${OUTPUTROOTNAME}
        cp ${INFILE} ${OUTPUTDIR}/${OUTPUTNAME}_${IFILE}.root
    else
        if [ "x${X509_USER_PROXY}" == "x" ]; then
            echo "Copying outputs to Hadoop via cp."
            mkdir -p ${OUTPUTDIR}
            INFILE=${OUTPUTROOTNAME//.root/}
            INDEX=0
            for OUTPUTFILE in $(ls ${INFILE}*.root); do
                if [ $INDEX -lt 1 ]; then
                    echo "cp ${OUTPUTFILE} ${OUTPUTDIR}/${OUTPUTNAME}_${IFILE}.root"
                    cp ${OUTPUTFILE} ${OUTPUTDIR}/${OUTPUTNAME}_${IFILE}.root
                else
                    echo "cp ${OUTPUTFILE} ${OUTPUTDIR}/${OUTPUTNAME}_${IFILE}_${INDEX}.root"
                    cp ${OUTPUTFILE} ${OUTPUTDIR}/${OUTPUTNAME}_${IFILE}_${INDEX}.root
                fi
                INDEX=$((INDEX+1))
            done
        else
            echo 'ls -l'
            ls -l
            echo 'gfal-copy'
            INFILE=${OUTPUTROOTNAME//.root/}
            INDEX=0
            for OUTPUTFILE in $(ls ${INFILE}*.root); do
                if [ $INDEX -lt 1 ]; then
                    echo gfal-copy -p -f -t 4200 --verbose file://`pwd`/${OUTPUTFILE} gsiftp://gftp.t2.ucsd.edu/${OUTPUTDIR}/${OUTPUTNAME}_${IFILE}.root --checksum ADLER32
                    gfal-copy -p -f -t 4200 --verbose file://`pwd`/${OUTPUTFILE} gsiftp://gftp.t2.ucsd.edu/${OUTPUTDIR}/${OUTPUTNAME}_${IFILE}.root --checksum ADLER32
                else
                    echo gfal-copy -p -f -t 4200 --verbose file://`pwd`/${OUTPUTFILE} gsiftp://gftp.t2.ucsd.edu/${OUTPUTDIR}/${OUTPUTNAME}_${IFILE}_${INDEX}.root --checksum ADLER32
                    gfal-copy -p -f -t 4200 --verbose file://`pwd`/${OUTPUTFILE} gsiftp://gftp.t2.ucsd.edu/${OUTPUTDIR}/${OUTPUTNAME}_${IFILE}_${INDEX}.root --checksum ADLER32
                fi
                INDEX=$((INDEX+1))
            done
        fi
    fi
    if [ $? -eq 0 ]; then
        echo "Job Success"
    else
        echo "Job Failed"
    fi
fi
###################################################################################################

#eof
