//
//  SFuncNode.h
//  mm
//
//  Created by Anna-Monica  on 8/2/18.
//  Copyright © 2018 Anna-Monica. All rights reserved.
//

#ifndef SFuncNode_h
#define SFuncNode_h
#include "Node.h"
#include "Razorfunc.h"

//#define _CLV_
#ifdef _CLV_
#define DEBUG(a) std::cout<<a
#else
#define DEBUG(a)
#endif

using namespace std;
//takes care of functions with arguments

class SFuncNode : public Node {
private:
    //should add something related to trigger types
    Node* userObjectA;
    Node* userObjectB;
    double (*f)(AnalysisObjects*, string, int);
    double (*g1)(AnalysisObjects*, string, int, std::vector<TLorentzVector> (*func)(std::vector<TLorentzVector>));
    double (*g2)(AnalysisObjects*, string, int,                      double (*func)(std::vector<TLorentzVector>));
    double (*g3)(AnalysisObjects*, string, int,                      double (*func)(std::vector<TLorentzVector>, TVector2 ));
    double (*g4)(AnalysisObjects*, string, int,   TLorentzVector,    double (*func)(std::vector<TLorentzVector>, TLorentzVector ));
    std::vector<TLorentzVector> (*h1)(std::vector<TLorentzVector>);
               double (*h2)(std::vector<TLorentzVector>);
               double (*h3)(std::vector<TLorentzVector>, TVector2 );
               double (*h4)(std::vector<TLorentzVector>, TLorentzVector );
    int type;
    bool ext;
    std::vector<myParticle*> inputParticles;
public:
    SFuncNode(double (*func)(AnalysisObjects* ao, string s, int id), 
                      int id, 
               std::string s, 
               Node *objectNodeA = NULL, Node *objectNodeB = NULL) {
        f=func;
        g1=NULL;
        g2=NULL;
        g3=NULL;
        g4=NULL;
        ext=false;
        type=id;
        symbol=s;
        left=NULL;
        right=NULL;
        userObjectA = objectNodeA;
        userObjectB = objectNodeB;
    }
//-------------------------extern.........
    SFuncNode(double (*func)(AnalysisObjects* ao, string s, int id, std::vector<TLorentzVector> (*gunc) (std::vector<TLorentzVector> jets)),
              std::vector<TLorentzVector> (*tunc) (std::vector<TLorentzVector> jets),
                      int id, 
               std::string s, 
               Node *objectNodeA = NULL, Node *objectNodeB = NULL) {
        DEBUG("*****************************************EXTERN SF :"<<s <<"\n");
        f=NULL;
        g3=NULL;
        g2=NULL;
        g4=NULL;
        g1=func;
        h1=tunc;
        ext=true;
        type=id;
        symbol=s;
        left=NULL;
        right=NULL;
        userObjectA = objectNodeA;
        userObjectB = objectNodeB;
    }
    SFuncNode(double (*func)(AnalysisObjects* ao, string s, int id, double (*gunc) (std::vector<TLorentzVector> jets)),
              double (*tunc) (std::vector<TLorentzVector> jets),
                      int id, 
               std::string s, 
               Node *objectNodeA = NULL, Node *objectNodeB = NULL) {
        DEBUG("*****************************************EXTERN SF :"<<s <<"\n");
        f=NULL;
        g4=NULL;
        g3=NULL;
        g1=NULL;
        g2=func;
        h2=tunc;
        ext=true;
        type=id;
        symbol=s;
        left=NULL;
        right=NULL;
        userObjectA = objectNodeA;
        userObjectB = objectNodeB;
    }
    SFuncNode(double (*func)(AnalysisObjects* ao, string s, int id, double (*gunc) (std::vector<TLorentzVector> jets, TVector2 amet)),
              double (*tunc) (std::vector<TLorentzVector> jets, TVector2 amet),
                      int id, 
               std::string s, 
               Node *objectNodeA = NULL, Node *objectNodeB = NULL) {
        DEBUG("*****************************************EXTERN SF :"<<s <<"\n");
        f=NULL;
        g1=NULL;
        g2=NULL;
        g4=NULL;
        g3=func;
        h3=tunc;
        ext=true;
        type=id;
        symbol=s;
        left=NULL;
        right=NULL;
        userObjectA = objectNodeA;
        userObjectB = objectNodeB;
    }
    SFuncNode(double (*func)(AnalysisObjects* ao, string s, int id, TLorentzVector alv, double (*gunc) (std::vector<TLorentzVector> jets, TLorentzVector amet)),
              double (*tunc) (std::vector<TLorentzVector> jets, TLorentzVector amet),
                      int id, 
               std::string s,
               std::vector<myParticle*> input, 
               Node *objectNodeA = NULL, Node *objectNodeB = NULL) {
        DEBUG("*****************************************EXTERN SF :"<<s <<"\n");
        f=NULL;
        g1=NULL;
        g2=NULL;
        g3=NULL;
        g4=func;
        h4=tunc;
        ext=true;
        type=id;
        symbol=s;
        left=NULL;
        right=NULL;
        inputParticles=input;
        userObjectA = objectNodeA;
        userObjectB = objectNodeB;
    }


//---------------------------end of externs
virtual void setSymbol(string s) { symbol=s; }
    
virtual double evaluate(AnalysisObjects* ao) override {
        DEBUG("*******In SF Eval TF:"<< ext <<"\n"); 
        if(userObjectA && !ext) {
           DEBUG("\t a user obj\n"); 
           userObjectA->evaluate(ao); // returns 1, hardcoded. see ObjectNode.cpp
           DEBUG("user obj done.\n"); 
        }
        if(userObjectB && !ext) {
           DEBUG("\t b user obj\n"); 
           userObjectB->evaluate(ao); // returns 1, hardcoded. see ObjectNode.cpp
           DEBUG("user obj done.\n"); 
        }
        dbxParticle *aPart= new dbxParticle;

        if ( inputParticles.size()>0 ){
           aPart->Reset();
           TLorentzVector ametlv;
           DEBUG("\t input particles \n"); 
           for(vector<myParticle*>::iterator i=inputParticles.begin();i!=inputParticles.end();i++){
               DEBUG("type:"<<(*i)->type<<" index:"<< (*i)->index<< " addr:"<<*i<<  "\t name:"<< (*i)->collection<<"\n");
              int atype=(*i)->type;
                int ai=(*i)->index;
             string ac=(*i)->collection;
             if (atype==7) ac="MET";

               switch (atype) { 
                   case 0:  aPart->setTlv(  aPart->lv()+ao->muos[ac].at(ai).lv() );   break;
                   case 1:  aPart->setTlv(  aPart->lv()+ao->eles[ac].at(ai).lv() );   break;
                  case 11:  aPart->setTlv(  aPart->lv()+ao->taus[ac].at(ai).lv() );   break;
                   case 2:  aPart->setTlv(  aPart->lv()+ao->jets[ac].at(ai).lv() );   break;
                   case 9:  aPart->setTlv(  aPart->lv()+ao->ljets[ac].at(ai).lv() );  break;
                   case 8:  aPart->setTlv(  aPart->lv()+ao->gams[ac].at(ai).lv() );   break;
                   case 7: DEBUG("MET LV\n ");
                            ametlv.SetPxPyPzE(ao->met[ac].Px(), ao->met[ac].Py(), 0, ao->met[ac].Mod());
                            aPart->setTlv(aPart->lv()+ametlv); // v from MET using same eta approx.
                            break;
                   default: std::cout<<"No such object! ERROR\n";
                            break;

               } // end of switch
           }// end of for

           DEBUG("aPart constructed \t");
        }

        std::vector<TLorentzVector> abc;       
        DEBUG("*****************EXTERN TF eval:"<< ext <<"\n");
        if(ext) { 
               DEBUG("external user function evaluate\n");
              if (g1 != NULL) return (*g1)(ao, symbol, type, h1 );
              if (g2 != NULL) return (*g2)(ao, symbol, type, h2 );
              if (g3 != NULL) return (*g3)(ao, symbol, type, h3 );
              if (g4 != NULL) return (*g4)(ao, symbol, type, aPart->lv(), h4);
        }               
        return (*f)(ao, symbol, type);
}

    virtual void Reset() override{}
    virtual void getParticles(std::vector<myParticle *>* particles) override{}
    virtual void getParticlesAt(std::vector<myParticle *>* particles, int index) override{}
    virtual ~SFuncNode() {}
};

double all(AnalysisObjects* ao, string s, int id){
    return 1;
}
double count(AnalysisObjects* ao, string s, int id) {
    particleType pid = (particleType)id;

    DEBUG("STR:"<<s<<" Type:"<<id<< "#J types:"<<ao->jets.size() << " #P types:"<<ao->gams.size()<<"\n");
    map <string, std::vector<dbxJet>  >::iterator it;
    for (it=ao->jets.begin();it!=ao->jets.end();it++){
      DEBUG("\t #Jtypename:"<<it->first<<"    size:"<<it->second.size() <<"\n");
    }
    map <string, std::vector<dbxPhoton>  >::iterator itp;
    for (itp=ao->gams.begin();itp!=ao->gams.end();itp++){
      DEBUG("\t #Ptypename:"<<itp->first<<"    size:"<<itp->second.size() <<"\n");
    }

    switch (pid) {
     case muon_t:     return (ao->muos.at(s).size()); break;
     case electron_t: return (ao->eles.at(s).size()); break;
     case tau_t:      return (ao->taus.at(s).size()); break;
     case jet_t:      return (ao->jets.at(s).size()); break;
     case ljet_t:     return (ao->ljets.at(s).size()); break;
     case photon_t:   return (ao->gams.at(s).size()); break;
     default:         return (-1); break;
    }
    return (-1);
}
double met(AnalysisObjects* ao, string s, int id){
     DEBUG("MET:" << ao->met["MET"].Mod() <<"\n");
    return ( ao->met["MET"].Mod() );
}

double ht(AnalysisObjects* ao, string s, int id){
    double sum_htjet=0;
    for (UInt_t i=0; i<ao->jets.at(s).size(); i++) sum_htjet+=ao->jets.at(s).at(i).lv().Pt();
    return (sum_htjet  );       
}

//                                                           double                  sin   (            double x           );

double userfuncA(AnalysisObjects* ao, string s, int id, std::vector<TLorentzVector> (*func)(std::vector<TLorentzVector> jets ) ){
// string contains what to send
// id contains the particle type ASSUME ID=JET TYPE,

   DEBUG("Userfunction g1 :"<<s<<"\n");

   std::vector<TLorentzVector> myjets;
   for (UInt_t i=0; i<ao->jets.at(s).size(); i++) myjets.push_back(ao->jets.at(s).at(i).lv() );
   DEBUG("evaluating external function :"<<s<<"\n");
   std::vector<TLorentzVector> retjets= (*func)(myjets);
   DEBUG("external function Done. size:"<<retjets.size()<<"\n");
   for (int ipart=ao->jets.at(s).size()-1; ipart>=0; ipart--){ // I have all particles, jets, in an event.
     if (ipart > (retjets.size()-1) ) { 
         (ao->jets).find(s)->second.erase( (ao->jets).find(s)->second.begin()+ipart);
     } else {
          ao->jets.at(s).at(ipart).setTlv( retjets[ipart] );
     } 
   }
    return (1);       
}
double userfuncB(AnalysisObjects* ao, string s, int id, double (*func)(std::vector<TLorentzVector> jets ) ){
// string contains what to send
// id contains the particle type ASSUME ID=JET TYPE,

   DEBUG("Userfunction g2 :"<<s<<"\n");

   std::vector<TLorentzVector> myjets;
   for (UInt_t i=0; i<ao->jets.at(s).size(); i++) myjets.push_back(ao->jets.at(s).at(i).lv() );
   DEBUG("evaluating external function :"<<s<<"\n");
   double retvalue= (*func)(myjets);
   return (retvalue);       
}
double userfuncC(AnalysisObjects* ao, string s, int id, double (*func)(std::vector<TLorentzVector> jets, TVector2 amet ) ){
// string contains what to send
// id contains the particle type ASSUME ID=JET TYPE,

   DEBUG("Userfunction g3 :"<<s<<"\n");

   std::vector<TLorentzVector> myjets;
        TVector2 mymet=ao->met["MET"];
   for (UInt_t i=0; i<ao->jets.at(s).size(); i++) myjets.push_back(ao->jets.at(s).at(i).lv() );
   DEBUG("evaluating external function :"<<s<<"\n");
   double retvalue= (*func)(myjets, mymet);
   return (retvalue);       
}

double userfuncD(AnalysisObjects* ao, string s, int id, TLorentzVector alv, double (*func)(std::vector<TLorentzVector> jets, TLorentzVector amet ) ){
// string contains what to send
// id contains the particle type ASSUME ID=JET TYPE,

   DEBUG("Userfunction g4 :"<<s<<"\n");

   std::vector<TLorentzVector> myjets;
   for (UInt_t i=0; i<ao->jets.at(s).size(); i++) myjets.push_back(ao->jets.at(s).at(i).lv() );
   DEBUG("evaluating external function :"<<s<<"\n");
   double retvalue= (*func)(myjets, alv);
   return (retvalue);       
}







/*
double nbjets(AnalysisObjects* ao){
    ValueNode abc=ValueNode();
//    return (abc.tagJets(ao, 1).size() );
    return (1);
}

double nljets(AnalysisObjects* ao){
    ValueNode abc=ValueNode();
//    return (abc.tagJets(ao, 0).size() );
    return (1);
}


double emwt(AnalysisObjects* ao){
    double theLeptonTrkPhi = ao->eles.at(0).lv().Phi();
    double           leppt = ao->eles.at(0).lv().Pt();
    double dphi_e_et = fabs(theLeptonTrkPhi - ao->met.Phi());
    if (dphi_e_et>M_PI) dphi_e_et=2*M_PI-dphi_e_et;
    float mwt=sqrt(2*leppt*ao->met.Mod()*(1-cos(dphi_e_et)));
    return (mwt);
} 

double mmwt(AnalysisObjects* ao){
    double theLeptonTrkPhi = ao->muos.at(0).lv().Phi();
    double           leppt = ao->muos.at(0).lv().Pt();
    double dphi_e_et = fabs(theLeptonTrkPhi - ao->met.Phi());
    if (dphi_e_et>M_PI) dphi_e_et=2*M_PI-dphi_e_et;
    float mwt=sqrt(2*leppt*ao->met.Mod()*(1-cos(dphi_e_et)));
    return (mwt);
} 

double mmetmwt(AnalysisObjects* ao){
    double theLeptonTrkPhi = ao->muos.at(0).lv().Phi();
    double           leppt = ao->muos.at(0).lv().Pt();
    double dphi_e_et = fabs(theLeptonTrkPhi - ao->met.Phi());
    if (dphi_e_et>M_PI) dphi_e_et=2*M_PI-dphi_e_et;
    float mwt=sqrt(2*leppt*ao->met.Mod()*(1-cos(dphi_e_et)));
    return (mwt+ao->met.Mod() );
}

double emetmwt(AnalysisObjects* ao){
    double theLeptonTrkPhi = ao->eles.at(0).lv().Phi();
    double           leppt = ao->eles.at(0).lv().Pt();
    double dphi_e_et = fabs(theLeptonTrkPhi - ao->met.Phi());
    if (dphi_e_et>M_PI) dphi_e_et=2*M_PI-dphi_e_et;
    float mwt=sqrt(2*leppt*ao->met.Mod()*(1-cos(dphi_e_et)));
    return (mwt+ao->met.Mod() );
}
*/

#endif /* SFuncNode_h */
