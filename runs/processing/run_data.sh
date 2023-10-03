directory="/Users/asilar/SMPVJ_ANA/SinglePhoton_UL/Run2017B/"
for file in "$directory"/*; do
	CLA $file CMSNANO -i ../gjets_adl/exDATA.adl -e 100 
	#CLA $file CMSNANO -i ../gjets_adl/exDATA_LbnT.adl -e 100 
done
