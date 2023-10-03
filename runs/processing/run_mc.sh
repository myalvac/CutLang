export QCD_HT50to100="7.087601612772315"
export QCD_HT100to200="0.434891397504290"
export QCD_HT200to300="0.036357732462106"
export QCD_HT300to500="0.007471797303885"
export QCD_HT500to700="0.000836582873922"
export QCD_HT700to1000="0.000195446666531"
export QCD_HT1000to1500="0.000110143251016"
export QCD_HT1500to2000="0.000014061509170"
export QCD_HT2000toInf="0.000005359175387"
export GJets_DR_0p4_HT_100To200="0.000501644395110"
export GJets_DR_0p4_HT_200To400="0.000033318730935"
export GJets_DR_0p4_HT_400To600="0.000013986789023"
export GJets_DR_0p4_HT_600ToInf="0.000004959049130"
export G1Jet_LHEGpT_150To250="0.000019472657703"
export G1Jet_LHEGpT_250To400="0.000004502408764"
export G1Jet_LHEGpT_400To675="0.000001697870136"
export G1Jet_LHEGpT_675ToInf="0.000000497180038"

directory="/Users/asilar/SMPVJ_ANA/QCD_HT/"
count=0
for file in "$directory"/*; do
	echo $file
	count=$((count + 1))
	cp ../gjets_adl/exTEMPLATE_QCDtight.adl exQCD__tight$count.adl 
	cp ../gjets_adl/exTEMPLATE_QCDLbnT.adl exQCD__LbnT$count.adl 
	dir_name=$(basename "$file")
	result=$(echo "$dir_name" | cut -d "_" -f 1-2)
	sed "s/XSECTEMP/${!result}/g" exQCD__tight$count.adl > exQCD__tight${count}_modified.adl
	sed "s/XSECTEMP/${!result}/g" exQCD__LbnT$count.adl > exQCD__LbnT${count}_modified.adl
	rm exQCD__tight${count}.adl
	rm exQCD__LbnT${count}.adl
	#CLA $file CMSNANO -i exQCD__tight${count}_modified.adl
	#CLA $file CMSNANO -i exQCD__LbnT${count}_modified.adl
done

#
#
#       directory="/Users/asilar/SMPVJ_ANA/GJets_HT/"
#       count=0
#       for file in "$directory"/*; do
#       	#echo $file
#       	count=$((count + 1))
#       	cp ../gjets_adl/exTEMPLATE_tightMatched.adl exGJets__tightMatched$count.adl 
#       	dir_name=$(basename "$file")
#       	result=$(echo "$dir_name" | cut -d "_" -f 1-5)
#       	sed "s/XSECTEMP/${!result}/g" exGJets__tightMatched$count.adl > exGJets__tightMatched${count}_modified.adl
#       	rm exGJets__tightMatched${count}.adl
#       	#CLA $file CMSNANO -i exGJets__tightMatched${count}_modified.adl --deps
#       done
#
#       directory="/Users/asilar/SMPVJ_ANA/G1Jet/"
#       count=0
#       for file in "$directory"/*; do
#       	echo $file
#       	count=$((count + 1))
#       	cp ../tutorials/exTEMPLATE.adl exG1Jet$count.adl 
#       	dir_name=$(basename "$file")
#       	result=$(echo "$dir_name" | cut -d "_" -f 1-3)
#       	sed "s/XSECTEMP/${!result}/g" exG1Jet$count.adl > exG1Jet${count}_modified.adl
#       	rm exG1Jet${count}.adl
#       	#CLA $file CMSNANO -i exGJets${count}_modified.adl --deps
#       done
#
#




