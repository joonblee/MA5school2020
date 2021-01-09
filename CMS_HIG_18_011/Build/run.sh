source setup.sh

cd ../../../CMSSW_7_6_7/src/
eval `scram runtime -sh`
cd ../../madanalysis5/CMS_HIG_18_011/Build/
make

for MASS in 40 #20 40 60
do
  ./MadAnalysis5job ../Input/M${MASS}.txt &>M${MASS}.log&
  disown -a
done

