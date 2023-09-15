#ifndef DBX_PARTICLE_H
#define DBX_PARTICLE_H

#include "TLorentzVector.h"
#include "TTree.h"
#include <iostream>
#include <map>
#include <vector>
#define CHMAX 256


#ifndef USE_MEMCOPY
#define USE_MEMCOPY 0
#endif


struct analy_struct {   // Declare analysis types
	int BPcount;
	int Dumpcount;
	int maxEvents;
	int startpt;
	int verbfreq;
	bool dosystematics;
	bool doQCD;
	bool doRS;
   	std::string hlt;
	std::string inputtype;
        std::string dependencies;
};

struct syst_struct {   // Declare analysis types
        int index;
        std::string vartype;
        std::string varname;
        std::string systname;
        int varid;
};

typedef struct evt_data
{
	evt_data()
	{
	}

	void operator=( const evt_data &right)
	{

#if USE_MEMCOPY

		memcpy((void *)this,&right,sizeof(evt_data));
#else

		run_no       =right.run_no;
		lumiblk_no   =right.lumiblk_no;
		event_no     =right.event_no;
		ChannelNo    =right.ChannelNo;
		RunYear      =right.RunYear;
		TRG_e        =right.TRG_e;
		TRG_m        =right.TRG_m;
		TRG_j        =right.TRG_j;
		badjet       =right.badjet;
		top_hfor_type=right.top_hfor_type;
		mcevt_weight =right.mcevt_weight;
	 m_HF_Classification =right.m_HF_Classification;
		user_evt_weight =right.user_evt_weight;
		isemuoverlap =right.isemuoverlap;
		vxp_maxtrk_no=right.vxp_maxtrk_no;
		vxpType      =right.vxpType;
		pileup_weight=right.pileup_weight;
		bad_lar_feb  =right.bad_lar_feb;
		met_loose    =right.met_loose;
		maxEvents    =right.maxEvents;
		HLT_IsoMu17_eta2p1_LooseIsoPFTau20   =right.HLT_IsoMu17_eta2p1_LooseIsoPFTau20;
		core_Flags   =right.core_Flags;
		z_vtx_weight =right.z_vtx_weight;

                weight_mc    =right.weight_mc;
                weight_pileup=right.weight_pileup;
              weight_leptonSF=right.weight_leptonSF;
             weight_bTagSF_77=right.weight_bTagSF_77;
      	    correction_weight=right.correction_weight;
      	    luminosity_weight=right.luminosity_weight;
		 weight_xsec =right.weight_xsec; 
                weight_jvt   =right.weight_jvt;
       weight_sherpa_22_vjets=right.weight_sherpa_22_vjets;
#endif
	}
	unsigned int run_no, RunYear;
	unsigned int lumiblk_no, ChannelNo;
	unsigned int event_no;
	bool TRG_e, TRG_m, TRG_j;
	bool badjet;
	int  top_hfor_type, m_HF_Classification;
	float mcevt_weight;
	bool isemuoverlap;
	unsigned int vxp_maxtrk_no;
	int vxpType;
	float pileup_weight;
	bool bad_lar_feb;
	TVector2 met_loose;
	unsigned int maxEvents;
	bool HLT_IsoMu17_eta2p1_LooseIsoPFTau20;
	unsigned int core_Flags;
	float z_vtx_weight;
        double user_evt_weight;

   Float_t         weight_mc;
   Float_t         weight_pileup;
   Float_t         weight_leptonSF;
   Float_t         weight_bTagSF_77;
   Float_t         correction_weight;
   Float_t         luminosity_weight;
   Float_t         weight_xsec;
   Float_t         weight_jvt;
   Float_t         weight_sherpa_22_vjets;


}evt_data;


class dbxParticle : public TObject  {

public:
	dbxParticle();
	dbxParticle(TLorentzVector);
	dbxParticle(TLorentzVector, int);
	~dbxParticle();
	dbxParticle  operator+ (dbxParticle& c);
	static double deltaR(dbxParticle,dbxParticle);
	static double deltaPhi(dbxParticle,dbxParticle);
	//dbxParticle  operator-() const;
	static bool comparePt(dbxParticle lhs, dbxParticle rhs) { return (lhs.lv().Pt() > rhs.lv().Pt()); }
	void Reset(){ p_charge=0; p_pdgID=0; p_flavor=0; p_istight=0;
                      p_lvector.SetPtEtaPhiM(0, 0, 0, 0); p_attribute.clear(); 
                    }
	void dump();
	void dump_b ();
	void dumpLHCO(std::ofstream&  );

	int setCharge( int);
	int setPdgID( int);
	int setEtCone( double );
	int setPtCone( double );
	int setFlavor ( double );
	int setIsTight ( int );
	int setIsMedium ( bool );
	int setIsLoose ( bool );

	int setParticleIndx ( int );
	int setTlv( TLorentzVector );
       void setPPPE(float px, float py, float pz, float Ee ){p_lvector.SetPxPyPzE(px, py, pz, Ee ); }
       void addTlv(TLorentzVector *alv){p_lvector.SetPxPyPzE( p_lvector.Px()+alv->Px(),  p_lvector.Py()+alv->Py(),  p_lvector.Pz()+alv->Pz(),  p_lvector.E()+alv->E()); }
       void addTlv(TLorentzVector alv){p_lvector.SetPxPyPzE( p_lvector.Px()+alv.Px(),  p_lvector.Py()+alv.Py(),  p_lvector.Pz()+alv.Pz(),  p_lvector.E()+alv.E()); }
	int scaleLorentzVector ( double );
	int scalePt ( double );
	int scaleE ( double );
	int setZ0 (double );
	void setAttribute(int k, double v) {
                  if (k>(int)p_attribute.size()) { std::cerr<<"NO Such Attribute! Use addAttribute first.\n";
                  } else { p_attribute[k]=v; }
        }
	void addAttribute(double v) {p_attribute.push_back(v);}

	int q()  { return p_charge; }
	int pdgID() {return p_pdgID; }
	double Attribute(int k)  { if (k>(int)p_attribute.size()){
                                 std::cerr<<"NO Such Attribute! ID:"<<k<<"\n";return -999999;} else {return p_attribute.at(k);} }
	int nAttribute() { return p_attribute.size(); }
	double EtCone()  { return p_et_cone; }
	double PtCone()  { return p_pt_cone; }
	double Flavor()  { return p_flavor; }
	int    isTight() { return p_istight; }
	bool    isMedium(){ return p_ismedium; }
	bool    isLoose() { return p_isloose; }
	int    ParticleIndx() { return p_particleindx; }
	TLorentzVector lv()  const{ return p_lvector; }
	double Z0() {return p_z0;}


private:
        std::vector<double> p_attribute;
	int p_charge;
	int p_pdgID;
	double p_et_cone;
	double p_pt_cone;
	double p_flavor;
	int p_particleindx;
	TLorentzVector p_lvector;
	 int p_istight;
	bool p_ismedium;
	bool p_isloose;
	double p_z0;

	ClassDef(dbxParticle,2);
};

#endif
