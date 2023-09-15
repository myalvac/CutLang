#include "SearchNode.h"

//#define _CLV_
#ifdef _CLV_
#define DEBUG(a) std::cout<<a
#else
#define DEBUG(a)
#endif

using namespace std;

void SearchNode::performInnerOperation(vector<int> *v,vector<int> *indices, double *current_difference,AnalysisObjects* ao){
          
        for(int i=0;i<v->size();i++){
            DEBUG(v->at(i)<<" ");
            particles.at(indices->at(i))->index=v->at(i);
        }
        double tmpval=left->evaluate(ao); // enabling this makes total 1min6s, without it 12s
        double diff=right->evaluate(ao)-tmpval;

        if ( (*f)(diff,*current_difference) ) {
             DEBUG("diff:"<<diff<<" c_diff:"<<*current_difference<<"\n");
             *current_difference = fabs(diff);
             bestIndices=*v;
        } else { DEBUG("\n"); }
      }

void SearchNode::runNestedLoopBarb( int start, vector<int> Ns, int level, int maxDepth, vector<int> *v,
                                    vector<int> *indices,double *curr_diff,AnalysisObjects* ao, vector<int> types, vector<string> acs) {
DEBUG("------------------------------------------------new search ----------------------------------------------------------------\n");
      const int unk_MAX=8; // max unknowns
      bool found_at_least_one=false;
      int ip_N[unk_MAX]; 
      int tip[unk_MAX]; 
      int  oi[unk_MAX]; 
/*
      for (int kk=0; kk<Ns.size(); kk++){
          ip_N[kk]=Ns[kk];
            oi[kk]=Ns[kk];
           tip[kk]=types[kk];
         DEBUG(kk<< " t:"<<types[kk]<<"\n");
      }
      for (int kk=Ns.size(); kk<unk_MAX; kk++){
          ip_N[kk]=ip_N[kk-1];
            oi[kk]=oi[kk-1];
           tip[kk]=tip[kk-1];
         DEBUG(kk<< " t2:"<<tip[kk-1]<<"\n");
      }
*/
      int unkidx=0;  
      int knowns=0;  
      for (int kk=0; kk<unk_MAX; kk++){ // up to max
           if (kk < particles.size() ) {
             if (particles.at(kk)->index < 0){  // a particle I will be searching
                oi[unkidx]=particles.at(kk)->index;
              ip_N[unkidx]=Ns[unkidx];
               tip[unkidx]=types[unkidx];
                  unkidx++;
             } 
           }
        }
      for (int i=maxDepth; i<unk_MAX; i++)  { ip_N[i]=1; tip[i]=1;} 
      for (int kk=0; kk<Ns.size(); kk++){
       DEBUG("oi:"<<oi[kk]<<"  type:"<<tip[kk]<< " maxN:"<< ip_N[kk]<< " order:"<<indices->at(kk)<<"\n" );
      }


     int ip[unk_MAX]; 
      int ip2_min=0, ip3_min=0, ip4_min=0, ip5_min=0, ip1_min=0;


      int forbidit_size;
      std::map<string, pair <int, unordered_set<int> > >::iterator forbidit;
//-----TODO: put a check mechanism
      for (int kk=0; kk<Ns.size(); kk++){
       string ac=acs[kk];
       if ( FORBIDDEN_INDEX_LIST.find(ac) !=FORBIDDEN_INDEX_LIST.end() ){
        DEBUG("FORBIDDEN_INDEX_LIST is NOT Empty for:"<< ac<<".\n");
        forbidit=FORBIDDEN_INDEX_LIST.find(ac);
        forbidit_size=forbidit->second.second.size(); // check the type!!!!!!!!!!!!
         DEBUG("type:"<<forbidit->second.first<<"\t");
        tip[kk]=forbidit->second.first;
       } else {
        DEBUG("FORBIDDEN_INDEX_LIST is Empty for:"<<ac<<".\n");
        forbidit_size=0;
       }
      }

// -----------for more debugging
/*
      for (int kk=0; kk<Ns.size(); kk++){
       string s=particles.at(kk)->collection;
       DEBUG(kk<<" |:"<<maxDepth<<" N:"<<Ns[kk]<< " #ForbiddenIndexSize:"<<forbidit_size
                  << " Type:"<<types[kk]<<" Collection:"<<s<<" ac:"<<acs[kk]<<"\n");
      }
*/

      string s=particles.at(0)->collection;
      if ((types[0]==combo_t) && ( ao->combosA.find(s) != ao->combosA.end() ) ) // only for 0 NGU FIXME
       if ( ao->combosA[s].tableA.size() > 0){
       DEBUG("-------------comboA-------------------\n");
        for (int k1=0; k1<ao->combosA.at(s).tableA.size(); k1++) {
         if (ao->combosA.at(s).tableA[k1].size()==maxDepth) {
         for (int k2=0; k2<ao->combosA.at(s).tableA[k1].size(); k2++) {
          DEBUG(ao->combosA.at(s).tableA[k1][k2] << " ");
// the indice @ k1,k2 should be used
          if (k2 < maxDepth) v->push_back( ao->combosA.at(s).tableA[k1][k2] );
         }
         for(int i=0;i<v->size();i++){
                      particles.at(indices->at(i))->index=v->at(i);
         }   
         double tmpval=left->evaluate(ao); 
         double diff=right->evaluate(ao)-tmpval;

         if ( (*f)(diff,*curr_diff) ) {
                       DEBUG("diff:"<<diff<<" c_diff:"<<*curr_diff<<"\n");
                       *curr_diff = fabs(diff);
                       bestIndices.clear();
                       for(int i=0;i<v->size();i++){ bestIndices.push_back(v->at(i) ); }
                 } else { DEBUG("\n");}
          v->clear();
         }
        }
      return;
      }
      DEBUG("NON COMBO regular, maxDepth:"<<maxDepth<<"\n");
      // loops start ~~~~~~~~~~~
      DEBUG("MAX ips:"<< ip_N[0]<< " "<<ip_N[1]<<" "<<ip_N[2]<<" "<< ip_N[3]<<" "<<ip_N[4]<<" "<<ip_N[5]<<"\n");
      unordered_set<int> Forbidden_Indices;
      if (forbidit_size > 0) Forbidden_Indices=forbidit->second.second;
          DEBUG("Before LOOP, Nb Particles:"<< particles.size() <<"\n");
          for (ip[0]=0; ip[0]<ip_N[0]; ip[0]++) {
           DEBUG("0 i:"<<ip[0]<<" t:"<< tip[0]<<"\n");
           if ( Forbidden_Indices.find( ip[0] )!=Forbidden_Indices.end() ) continue;        
           for (ip[1]=ip1_min; ip[1]<ip_N[1]; ip[1]++) {
           DEBUG("1 i:"<<ip[1]<<" t:"<< tip[1] <<"\n");
            if (Forbidden_Indices.find( ip[1] )!=Forbidden_Indices.end() ) { DEBUG("FORBIDDEN\n"); continue; }        
            if (particles.size()>1 && (ip[1]==ip[0]) && (tip[0]==tip[1])) { DEBUG("Repeated \n"); continue; }
            if ( (oi[0] == oi[1] ) && (ip[0]>ip[1]) ) { DEBUG("Same OI, repeated\n"); continue; }

              for (ip[2]=ip2_min; ip[2]<ip_N[2]; ip[2]++) {
                DEBUG("2 i:"<<ip[2]<<" t:"<< tip[2] <<"\n");
                if ( maxDepth>2 ){ 
                   if ( particles.size()>2 && ((ip[2]==ip[0] && tip[2]==tip[0]) || (ip[2]==ip[1] && tip[2]==tip[1]) )) continue; // CHECK TYPES.
                   if ( Forbidden_Indices.find( ip[2] )!=Forbidden_Indices.end() ) continue;        
                }
                for (ip[3]=ip3_min; ip[3]<ip_N[3]; ip[3]++) {
                  DEBUG("3 i:"<<ip[3]<<" t:"<< tip[3] <<"\n");
                  if ( maxDepth>3){ 
                     if ( particles.size()>3 && ((ip[3]==ip[0] && tip[3]==tip[0]) || (ip[3]==ip[1] && tip[3]==tip[1] )|| (ip[3]==ip[2] && tip[3]==tip[2])) ) continue;
                     if ( Forbidden_Indices.find( ip[3] )!=Forbidden_Indices.end() ) continue;        
                     if ( (oi[2]==oi[3]) && (ip[2]>ip[3]) ) continue;
                  }
                  for (ip[4]=ip4_min; ip[4]<ip_N[4]; ip[4]++) {
                    DEBUG("4 i:"<<ip[4]<<" t:"<< tip[4] <<"\n");
                    if ( maxDepth>4)  {
                     if ( particles.size()>4 && ((ip[4]==ip[0] && tip[4]==tip[0]) || (ip[4]==ip[1] && tip[4]==tip[1]) 
                                             || ( ip[4]==ip[2] && tip[4]==tip[2]) || (ip[4]==ip[3] && tip[4]==tip[3]) )) continue;
                     if ( Forbidden_Indices.find( ip[4] )!=Forbidden_Indices.end() ) continue;        
                     if ( ( oi[3] == oi[4]) &&(ip[3]>ip[4]) )  continue;
                    }
                     for (ip[5]=ip5_min; ip[5]<ip_N[5]; ip[5]++) {
                       DEBUG("5 i:"<<ip[5]<<" t:"<< tip[5] <<"\n");
                      if (maxDepth>5) {
                       if ( particles.size()>5 &&(ip[5]==ip[0] && tip[5]==tip[0] ) || (ip[5]==ip[1] && tip[5]==tip[1] ) 
                                               ||(ip[5]==ip[2] && tip[5]==tip[2] ) || (ip[5]==ip[3] && tip[5]==tip[3] ) 
                                               ||(ip[5]==ip[4] && tip[5]==tip[4]  )) continue;
                       if ( Forbidden_Indices.find( ip[5] )!=Forbidden_Indices.end() ) continue;        
                       if ( ( oi[4] == oi[5]) &&(ip[4]>ip[5]) )  continue;
                      }

                      if ( (oi[3]== (-100+oi[0]) ) && (ip[0]>ip[3]) ) continue;
          
                 DEBUG("testing the combi::"<<ip[0]<<" "<<ip[1]<<" "<<ip[2]<<" "<<ip[3]<<" "<<ip[4]<<" "<<ip[5]<<"\n");
 //                cout<<"testing the combi::"<<ip[0]<<" "<<ip[1]<<" "<<ip[2]<<" "<<ip[3]<<" "<<ip[4]<<" "<<ip[5]<<"\n";
                 for (int i=0; i<maxDepth; i++) v->push_back(ip[i]);
                  DEBUG("New Set:\n");
                  for(int i=0;i<v->size();i++){
                      DEBUG(v->at(i)<<"\t --> i:"<<indices->at(i)<<"  was:"<<particles.at(indices->at(i))->index <<"\n" );
                      particles.at(indices->at(i))->index=v->at(i);
                  }
                  DEBUG("now left evaluate\n");

//-------~1min in 25k events
                 double tmpval=left->evaluate(ao); // enabling this gives another +20s in Brr's analysis
                 DEBUG("left returns:"<<tmpval<< "\t");
                 double diff=right->evaluate(ao)-tmpval;

                 DEBUG("right:"<<right->evaluate(ao)<<" compare: \n");
                 if ( (*f)(diff,*curr_diff) ) {
                       DEBUG("BETTER diff:"<<diff<<" OLD c_diff:"<<*curr_diff<<"\n");
                       *curr_diff = fabs(diff);
                       bestIndices.clear();
                       int ijkl;
                       for(int iq=0;iq<v->size();iq++){bestIndices.push_back( v->at(iq) ); }
                     found_at_least_one=true;
                 } else { DEBUG("OLD diff was better...\n"); }

                 v->clear();
                }}
             }}}} //all iN loops end

     DEBUG("Search Ended. best chi:"<< *curr_diff<<"\n");
    }

   void SearchNode::runNestedLoopRec( int start, int N, int level, int maxDepth, vector<int> *v,
                                     vector<int> *indices,double *curr_diff,AnalysisObjects* ao, int type, string ac) {
    if(level==maxDepth)  performInnerOperation (v,indices,curr_diff,ao); //18 without this function call
    else{
        bool skip=false;
        for (int x = start; x < N; x++ ) {

            skip=false;
            for (int kk=0; kk<v->size(); kk++){
             if (v->at(kk)==x) {
                 skip=true; break;}
             //check if particle x is forbidden
              std::cout<<"FIXME\n";
                    {skip=true; break;}
            }
            if (skip) continue;

            v->push_back(x); //add the current value
            runNestedLoopRec( start, N, level + 1, maxDepth, v,indices, curr_diff, ao, type, ac);
            v->pop_back();//remove the value
        }
    }
}


SearchNode::SearchNode(double (*func)(double, double), Node* l, Node* r, std::string s){
        f=func;
        symbol=s;
        left=l;
        right=r;
}

double SearchNode::evaluate(AnalysisObjects* ao) {
        DEBUG("\nSearchN ---------------"<<getStr()<<"\n"); 
        particles.clear();
        left->getParticles(&particles);//should fill with particles pointers no more cast needed
        
        std::map<string, std::pair<int, unordered_set<int> > >::iterator forbidit;
        vector<int> indices;
        for(int i=0;i<particles.size();i++){
             DEBUG("\t SearchN Part:"<<i<<" idx:"<<particles.at(i)->index<< " addr:"<<particles.at(i)<<" name:"<< particles.at(i)->collection<< " type:"<<particles.at(i)->type<<"\n");
            
             if(particles.at(i)->index<0) indices.push_back(i);

             else {
                forbidit=FORBIDDEN_INDEX_LIST.find( particles.at(i)->collection  ); 
                if (forbidit == FORBIDDEN_INDEX_LIST.end() ){
                     DEBUG( particles.at(i)->collection << " was NOT blacklisted, now adding with:"<<  particles.at(i)->index 
							<< " and type:" << particles.at(i)->type <<"\n");
                     unordered_set<int> pippo;
                                        pippo.insert(particles.at(i)->index);
                     pair<int, unordered_set<int> > tpippo(particles.at(i)->type, pippo);
                     FORBIDDEN_INDEX_LIST.insert(std::pair<string, pair<int, unordered_set<int> > >(particles.at(i)->collection, tpippo));
                } else { 
                        DEBUG( particles.at(i)->collection << " found, adding"<< particles.at(i)->index<<"\n"); 
			forbidit->second.second.insert(particles.at(i)->index);  
                }
             }
      }

        int MaxDepth=indices.size();//number of nested loops needed
        DEBUG("SearchN Depth:"<<MaxDepth<<" particles will be searched.\n");
        if(MaxDepth>0){
               vector<int> candids;
               vector<int> types;
               vector<string> collections;
               for(int i=0; i<MaxDepth;i++){
                    int type=particles.at(indices[i])->type;
                    string ac=particles.at(indices[i])->collection;
                    collections.push_back(ac);
                          types.push_back(type);
                    DEBUG("will search for type:"<<type<<"\n");
                    int Max;
                    switch(type){//assuming all par
                        case muon_t: Max=ao->muos.at(ac).size();break;
                        case electron_t: Max=ao->eles.at(ac).size();break;
                        case jet_t: Max=ao->jets.at(ac).size();break;
                        case fjet_t : Max=ao->ljets.at(ac).size();break;
                        case bjet_t: Max=left->tagJets(ao,1,ac).size();break;
                        case lightjet_t: Max=left->tagJets(ao,0,ac).size();break;
		        case photon_t: Max=ao->gams.at(ac).size();break;
			case tau_t: Max=ao->taus.at(ac).size();break;
			case truth_t: Max=ao->truth.at(ac).size();break;
			case combo_t: Max=ao->combos.at(ac).size();break;
                      default :
                              std::cout<<"optimizing for Unkown type... ERROR!\n";
                              exit(-11);
                              break;
                    }
                    DEBUG("Max candidate:"<<Max<<" collection:"<< ac<< " particle index:"<<i<<"\n");
                        candids.push_back(Max);
               }// end of loop over particles
 // ok up to here                  

                    DEBUG("Before find, # types:"<<candids.size()<< " Best index vector size:"<<bestIndices.size()<<"\n");
                    vector<int> v;//--------------------why not pass it by reference?!
                    double current_difference =9999999999.9;
                    runNestedLoopBarb( 0, candids, 0, MaxDepth, &v,&indices, &current_difference,ao, types, collections);
                   // runNestedLoopRec( 0, Max, 0, MaxDepth, &v,&indices, &current_difference,ao, type, ac);

                   DEBUG("After find, Best index vector size:"<<bestIndices.size()<<" MaxDepth"<<MaxDepth<<"\n");
                   if (bestIndices.size() < 1 ) {  
                         DEBUG("!!! Best index could not be found.\n"); 
                         double  leftval=left->evaluate(ao);
                         return 1; 
                   }
                    
                   int maxFound = bestIndices.size();
//                    if (MaxDepth < bestIndices.size()) maxFound = MaxDepth;
                   for(int i=0;i<maxFound;i++){
                        DEBUG("particle at:"<<indices[i]<<" will be replaced by:"<<bestIndices.at(i)<<"\n"); //directly changing the concerned particle -i-->bestIndices[i]
                        particles.at(indices[i])->index=bestIndices.at(i); //directly changing the concerned particle -i-->bestIndices[i]
                        //-------------------add found indices to FORBIDDEN 
                        string ac=collections[i];
                        forbidit=FORBIDDEN_INDEX_LIST.find( ac );
                        DEBUG("forbidding:"<<bestIndices[i]<<" for "<<ac<<"\n");
                        if (forbidit == FORBIDDEN_INDEX_LIST.end() ){
                         DEBUG(ac<<" was NOT there, now adding with "<< bestIndices[i] << " t:"<<types[i]<<"\n");
                         unordered_set<int> pippo;
                                            pippo.insert(bestIndices[i]);
                         std::pair<int, unordered_set<int> > tpippo (20, pippo); // NGU better type!!!!!!
                         FORBIDDEN_INDEX_LIST.insert(std::pair<string, std::pair<int, unordered_set<int> > >(ac, tpippo) );
                        } else {
                         forbidit->second.second.insert( bestIndices[i]);
                        }

                   }
        } else{
                DEBUG("SearchN No negative index found... Returning evaluation as is.\n");
                double  leftval=left->evaluate(ao); // enabling this makes total 1min6s, without it 12s
                DEBUG("SearchN left:"<<leftval<<"\t");
                double rightval=right->evaluate(ao);
                DEBUG(" right:"<<rightval<<"\n");
                if (leftval == rightval) { return 1; }
                else { std::cout<<"no negatif index, left is not right?!\n"; return 0;} // NGU TODO we should improve.
        }
        DEBUG("EOS. \n");
        particles.resize(6);
        return 1;
    }

    void SearchNode::Reset() {
            DEBUG("Clearing ForbiddenIndices on all names:\n");
            FORBIDDEN_INDEX_LIST.clear();
/*
            for (std::map<string, std::pair<int, unordered_set<int> > >::iterator it=FORBIDDEN_INDEX_LIST.begin(); it!=FORBIDDEN_INDEX_LIST.end(); ++it){
             DEBUG( it->first << " clearing \n"); 
             it->second.second.clear();
            }
*/
            bestIndices.clear();
            DEBUG("done.\n");
            left->Reset();
            right->Reset();
    }

    void SearchNode::getParticles(std::vector<myParticle *>* particles) {
     left->getParticles(particles);
    }
    void SearchNode::getParticlesAt(std::vector<myParticle *>* particles, int index) {
        
    }

    SearchNode::~SearchNode() {
//        if (left!=NULL) delete left;
//        if (right!=NULL) delete right;
    }
//-------set particles
   void SearchNode::setParticles(std::vector<myParticle *>* bankParticles) {
     particles.clear();
     for(int i=0;i<bankParticles->size();i++){
         DEBUG("Received Part:"<<i<<"  idx:"<<bankParticles->at(i)->index<< "  addr:"<<bankParticles->at(i)<<" name:"<< bankParticles->at(i)->collection<<"\n");
     }

     left->getParticles(&particles);//should fill with particles pointers no more cast needed
     int frombank=0;
     for(int i=0;i<particles.size();i++){
         DEBUG("Fill Part:"<<i<<"  idx:"<<particles.at(i)->index<< "  addr:"<<particles.at(i)<<" name:"<< particles.at(i)->collection<<"\n");
         if(particles.at(i)->index<0) {
                 particles.at(i)->index=bankParticles->at(frombank)->index;
                 frombank++;
                 DEBUG(i<<" Filled from Bank as "<< particles.at(i)->index <<".\n");
         }
     }
   }


//---------------------------------------------------------------------general functions
double maxim( double difference, double current_difference){
    return (fabs(difference) > current_difference );
}

double minim( double difference, double current_difference){
    DEBUG("dif:"<<difference <<"  cur_diff:"<<current_difference<<"\n");
    return fabs(difference) < current_difference ;
}
