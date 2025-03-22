#include "main.h"

class imp_res : public Restaurant
{
	public:	
		class Node{
			public:
			customer *cus;
            Node *next = nullptr;
            Node * prev = nullptr;
			bool sit = 0;
            long long thutu = 0;
            Node(customer*a){
                cus = a;
            }
		};
		class ordercus{
            public:
			long long thutu=0;
            Node* khach;
            ordercus* next;
			ordercus* prev;
            ordercus(){}
            ordercus(Node*a,ordercus *b,ordercus *c){
                this->khach = a;
                this->next = b;
				this->prev = c;
            }
            ~ordercus(){}
        };
		long long orderwait=0;
		int Nsort;
		int wait;
		int count;
        long long total;
        ordercus*headorder;
        ordercus*tailorder;
		customer *currentcus;
		Node *headwait;
		Node *tailwait;
		imp_res() {
			count = 0;
			wait = 0;
            total =0;
            headorder=nullptr;
            tailorder=nullptr;
			currentcus = nullptr;
			headwait = nullptr;
			tailwait = nullptr;
		};
        ~imp_res(){
            long long crttotal = total;
			ordercus * order = tailorder;
            for(int i=0;i<crttotal;i++){	
					ordercus* rmorder = order;
					order = order->prev;
					if(rmorder->khach->sit==0){
                        delete rmorder->khach->cus;
                        kickwait(rmorder->khach);
                        kickorder(rmorder);
                        }
                    else {
                        kick(rmorder->khach->cus);
                        delete rmorder->khach;
                        kickorder(rmorder);
                    }
            }	
		}
		void addcus(customer*cus){
		if(currentcus == nullptr){
			currentcus = cus;
			currentcus->next = currentcus;
			currentcus->prev = currentcus;
		}
		else{
			if (count < MAXSIZE/2){
				if(cus->energy >= currentcus->energy){
					customer * nextcus = currentcus->next;
					currentcus->next = cus;
					cus->next = nextcus;
					cus->prev =currentcus;
					nextcus->prev = cus;
					currentcus = cus;			
				}
				else{
					customer * prevcus = currentcus->prev;
					currentcus->prev = cus;
					cus->prev = prevcus;
					cus->next =currentcus;
					prevcus->next = cus;
					currentcus = cus;	
				}
			}
			else if(MAXSIZE/2 <= count && count < MAXSIZE){
				int absresmax = -1,resmax=0;
				customer * resmaxcus = nullptr;
				customer * crt = currentcus;
				for(int i =0;i<count;i++){
						int res = cus->energy - crt->energy;
						if (abs(res) >absresmax) {
							absresmax = abs(res);
							resmax = res;
							resmaxcus = crt;
							}
					crt=crt->next;
				}
				currentcus = resmaxcus;
				if(resmax>=0){
					customer * nextcus = currentcus->next;
					currentcus->next = cus;
					cus->next = nextcus;
					cus->prev =currentcus;
					nextcus->prev = cus;
					currentcus = cus;			
				}
				else{
					customer * prevcus = currentcus->prev;
					currentcus->prev = cus;
					cus->prev = prevcus;
					cus->next =currentcus;
					prevcus->next = cus;
					currentcus = cus;	
				}
			}
		}
		count ++;
		}
		void RED(string name, int energy)
		{	
            if(energy ==0) return;
			if(count == MAXSIZE && wait == MAXSIZE) return;
			customer *a = currentcus;
			Node *b = headwait;
			for(int i=0;i<count;i++){
				if(a->name == name) return;
				a = a->next;
			}
			for(int i=0;i<wait;i++){
				if(b->cus->name == name) return;
				b = b->next;
			}
			
			customer *cus = new customer (name, energy, nullptr, nullptr);
            Node * khach = new Node(cus);
            orderwait++;
            khach->thutu = orderwait;
            if(count < MAXSIZE) khach->sit =1;
			ordercus * order = new ordercus(khach,nullptr,nullptr);
            if(headorder == nullptr){headorder = order;tailorder = order;}
            else{
                tailorder->next=order;
				order->prev = tailorder;
                tailorder = order;
		}   
        total++;
		
        if(count<MAXSIZE) {addcus(cus);}
        else if (count == MAXSIZE && wait <MAXSIZE){
	    		if(headwait == nullptr){
				headwait = khach;
				tailwait = khach;
				}
				else {
				tailwait->next = khach;
				khach->prev = tailwait;
				tailwait = khach;  		
				}
				wait ++;
			}
			
		}
		void outwait(Node *a){
			Node* preva = a->prev ;
            Node* nexta = a->next ;
             if(a->prev == nullptr){
                            headwait=headwait->next;
							if(headwait!=nullptr)headwait->prev=nullptr;                           
            } else if (a->next==nullptr){
                            tailwait=tailwait->prev;
							if(tailwait!=nullptr) tailwait->next=nullptr;
            } else {    
                    	    preva->next = nexta;
                    	    nexta->prev = preva;
                    }
			a->next = nullptr;
			a->prev = nullptr;
			wait--;
            if(wait == 0){
				headwait = nullptr;
				tailwait = nullptr;
			}
            
		}
		void BLUE(int num)
		{
			int a = 0,b = 0;
			if(num<=count)a = num;
			else a = count;
            if(a<=wait)b=a;
			else b= wait;
            ordercus* order = headorder;
			while(a > 0){
                if(order->khach->sit == 1){
                    ordercus* rmordercus = order;
                    order = order->next;
				    kick(rmordercus->khach->cus);
                    delete rmordercus->khach;
				    kickorder(rmordercus);
                    a--;
                }
                else order = order->next;
            }
			for(int i =0;i<b;i++){
				Node* waiter =headwait;
                outwait(waiter);
                waiter->sit=1;
				addcus(waiter->cus);
			} 

		}
		void swap(Node*a,Node*b){
                Node* preva =a->prev;
				Node* nexta =a->next;
				Node* prevb =b->prev;
			    Node* nextb =b->next;
                 if(a->next ==b){              
                    b->next = a;
                    a->prev = b;
                } else{       
					b->next = nexta;
					nexta->prev = b;
					prevb->next = a;
					a->prev=prevb;
			    }
                if(preva != nullptr){
                    preva->next = b;
                    b->prev = preva;
                } else headwait =b, b->prev=nullptr;
                if(nextb !=nullptr){
                    a->next = nextb;
                    nextb->prev=a;
                } else tailwait =a, a->next = nullptr;
        }
        
        void Sortsegment(int size,int first,int k){ 
			int n = size - first;               
            for(int i =k;i<n;i+=k){
                Node* currentwait =headwait;
				for(int a =0;a <first+i;a++) currentwait = currentwait->next;
                for(int j=i;j>=k;j -=k){
				Node* prevwait =currentwait;
                for(int a =0;a <k;a++){
                    prevwait = prevwait->prev;
                }
                if(abs(prevwait->cus->energy)<abs(currentwait->cus->energy)){
					
                    swap(prevwait,currentwait);
					Nsort++;
                }
				else if(abs(prevwait->cus->energy)==abs(currentwait->cus->energy)&&prevwait->thutu>currentwait->thutu){
					swap(prevwait,currentwait);
					Nsort++;
				}
                else break;
               }             
            }

        }
		
		void PURPLE()
		{	this->Nsort = 0;
            int maxe=0; int vt=0;
            Node* waitcus = headwait;
            for(int i=1;i<=wait;i++){
                if(abs(waitcus->cus->energy) >= maxe) {maxe =abs(waitcus->cus->energy);vt =i;}
                waitcus=waitcus->next;
            }
            for(int i = vt/2;i>=1;i/=2)
                for(int j=0;j<i;j++){
                    Sortsegment(vt,j,i);
            }
			BLUE(Nsort%MAXSIZE);
		}
		void change(customer*a,customer*b){
					customer* preva =a->prev;
					customer* nexta =a->next;
					customer* prevb =b->prev;
					customer* nextb =b->next;
				if(count <=2) return;
                    if(a->next ==b){
                        preva->next = b;
                        b->next = a;
                        a->next = nextb;
                        a->prev = b;
                        nextb->prev = a;
                        b->prev = preva;
                    }
                    else if(b->next ==a){
                        prevb->next = a;
                        a->next = b;
                        b->next = nexta;
                        nexta->prev = b;
                        b->prev = a;
                        a->prev = prevb;
                    } else{
					preva->next = b;
					b->next = nexta;
					b->prev=preva;
					nexta->prev = b;
					prevb->next = a;
					a->next = nextb;
					a->prev=prevb;
					nextb->prev = a;}
		}
		void REVERSAL()
		{	if(currentcus ==nullptr) return;
			customer*Xrev= currentcus; 
            customer*tailXrev = currentcus->next;
			customer*first = currentcus;
			for(int i =1;i<=count;i++){
				if(Xrev->energy<0 && tailXrev->energy<0){
					if (Xrev == currentcus) first = tailXrev;
				    change(Xrev,tailXrev);
                    customer*a=Xrev;
                    Xrev = tailXrev;
                    tailXrev = a;
				    Xrev=Xrev->prev;
				    tailXrev=tailXrev->next;
                       i ++;
				}
			    else if(Xrev->energy>0) Xrev=Xrev->prev;
			    else if(tailXrev->energy>0) tailXrev=tailXrev->next;
				
            }
		    Xrev = first;
		    tailXrev = first->next;
		    for(int i =0;i<count;i++){
			    if(Xrev->energy>0 && tailXrev->energy>0){
				    change(Xrev,tailXrev);
                    customer*a=Xrev;
                    Xrev = tailXrev;
                    tailXrev = a;
				    Xrev=Xrev->prev;
				    tailXrev=tailXrev->next;
                    i ++;
				}
			    else if(Xrev->energy<0) Xrev=Xrev->prev;
			    else if(tailXrev->energy<0) tailXrev=tailXrev->next;
            }
		}
		class sum{
			public:
			int value;
			int vitri;
			sum * next;
			sum * prev;
			sum(){}
			sum(int a,int b){
				value =a;
				vitri = b;
				next = nullptr;
				prev = nullptr;
			}
		};
		class listsum{
			public:
			sum* headsum;
			sum* tailsum;
			int sumsize;
			listsum(){
				sumsize=0;
				headsum = nullptr;
				tailsum = nullptr;
			}
			~listsum(){
				sum* current = headsum;
				for(int i=0;i<sumsize;i++){
					sum* remove = current;
					current = current->next;
					delete remove; 
				}
				sumsize=0;
				headsum = nullptr;
				tailsum = nullptr;
			}
			void addfront(sum*a){
					if(headsum ==nullptr){
							headsum = a;
							tailsum = a;
					} else {
						headsum->prev =a;
						a->next = headsum;
						headsum = a;
					}
					sumsize ++;
			}
			void addback(sum*a){
					if(tailsum ==nullptr){
							headsum = a;
							tailsum = a;
					} else {
						tailsum->next =a;
						a->prev = tailsum;
						tailsum = a;
					}
					sumsize ++;
			}
			void removefront(){
				sum * remove = headsum;
				if(headsum == nullptr) return;
				headsum = headsum->next;
				if(headsum !=nullptr) headsum->prev = nullptr;
				sumsize--;
				delete remove;
				if(sumsize==0){
					headsum = nullptr;
					tailsum = nullptr;
				}
			}
			void removeback(){
				sum * remove = tailsum;
				if(tailsum ==nullptr) return;
				tailsum = tailsum->prev;
				if(tailsum !=nullptr) tailsum->next = nullptr;
				sumsize--;
				delete remove;
				if(sumsize==0){
					headsum = nullptr;
					tailsum = nullptr;
				}
			}
		};
		void UNLIMITED_VOID()
		{	customer* X = currentcus;
			if(count <4) return;
			int min = 0,vtmin = 0,dodai =4;
			customer * end = nullptr; 
			for(int i =0;i<4;i++){
				min+= X->energy;
				end = X;
				X = X->next;
			}
			sum* s = new sum(0,-1);
			listsum * sumlist = new listsum;
			sumlist->addfront(s);
			customer* start = currentcus;
			int total = min;
			int sumx = 0;
			customer* Y = currentcus;
			customer* lastvitri = X->prev;
			for(int i =4;i<count*2-1;i++){
				total += X->energy;
				sumx += Y->energy;
				sum* s1 = new sum(sumx,i-4);
				while(sumlist->sumsize != 0 && i- count > sumlist->headsum->vitri) sumlist->removefront();
				while(sumlist->sumsize != 0 && sumx > sumlist->tailsum->value) sumlist->removeback();
				sumlist->addback(s1);
				int sublen = i - sumlist->headsum->vitri;
				if(min > total- sumlist->headsum->value){
					dodai = sublen;
					min = total- sumlist->headsum->value;
					vtmin = sumlist->headsum->vitri + 1;
					lastvitri = X;
				}
				else if(min == total- sumlist->headsum->value && sublen>=dodai){
					
					if(vtmin <= (sumlist->headsum->vitri+1) % count ){
					dodai = sublen;
					min = total- sumlist->headsum->value;
					vtmin = sumlist->headsum->vitri + 1;
					lastvitri = X;}
				}
				X = X->next;Y = Y->next;
			}
			customer* minMin = lastvitri;
			X = lastvitri;
			int energymin = lastvitri->energy;
			int a =0;

			for(int i =0;i<dodai;i++){
					if(X->energy<=energymin) {
						energymin = X->energy;
						minMin = X; 
						a =i;
					}
					X= X->prev;
			}
			Y = minMin;
			for(int i =0;i<=a;i++){
				Y->print();
				Y = Y->next;
			}
			X= X->next;
			for(int i =0;i<dodai-a-1;i++){
				X->print();
				X = X->next;
			}
			delete sumlist;
		}
		void kick(customer* a){
            customer* preva = a->prev ;
            customer* nexta = a->next ;
            preva->next = nexta;
            nexta->prev = preva;
			if(a->energy<0) currentcus = a->prev;
            if(a->energy>0) currentcus = a->next;
            a->next = nullptr;
            a->prev=nullptr;
            delete a;
			count--;
			if(count == 0) currentcus = nullptr;
        }
        void kickwait(Node *a){
			Node* preva = a->prev ;
            Node* nexta = a->next ;
             if(a->prev == nullptr){
                            headwait=headwait->next;
							if(headwait!=nullptr)headwait->prev=nullptr;                           
            } else if (a->next==nullptr){
                            tailwait=tailwait->prev;
							if(tailwait!=nullptr) tailwait->next=nullptr;
            } else {    
                    	    preva->next = nexta;
                    	    nexta->prev = preva;
                    }
			a->next = nullptr;
			a->prev = nullptr;
            delete a;
			wait--;
			if(wait == 0){
				headwait = nullptr;
				tailwait = nullptr;
			}
        }
		void kickorder(ordercus *a){
			ordercus* preva = a->prev ;
            ordercus* nexta = a->next ;
             if(a->prev == nullptr){
                            headorder=headorder->next;
							if(headorder!=nullptr)headorder->prev=nullptr;                           
            } else if (a->next==nullptr){
                            tailorder=tailorder->prev;
							if(tailorder!=nullptr) tailorder->next=nullptr;
            } else {    
                    	    preva->next = nexta;
                    	    nexta->prev = preva;
                    }
            total--;
			a->next = nullptr;
			a->prev = nullptr;
            delete a;
			if(total ==0){
				headorder = nullptr;
				tailorder = nullptr;
			}
        }
		void remove(customer* a){
            customer* preva = a->prev ;
            customer* nexta = a->next ;
            preva->next = nexta;
            nexta->prev = preva;
			if(a->energy<0) currentcus = a->prev;
            if(a->energy>0) currentcus = a->next;
            a->next = nullptr;
            a->prev=nullptr;
			count--;
			if(count == 0) currentcus = nullptr;
        }
		
		void DOMAIN_EXPANSION()
		{	customer * dltlist = nullptr;
            customer* X = currentcus;
            int sum=0;
			int sumpos =0;
            for(int i=0;i<count;i++){
				if(X->energy>0)sumpos+=X->energy;
                sum +=X->energy;
                X=X->next;
            }
            Node* Y = headwait;
            for(int i=0;i<wait;i++){
				if(Y->cus->energy>0)sumpos+=Y->cus->energy;
                sum+=Y->cus->energy;
                Y=Y->next;
            }
              bool a;
			  sum = abs(sum);
            if (sumpos>=sum) a =1;
			else a =0;
			int crttotal=total;
			ordercus * order = headorder;
            for(int i=0;i<crttotal;i++){	
                 if((a==1 && order->khach->cus->energy<0)||(a==0 && order->khach->cus->energy>0)){
					ordercus* rmorder = order;
					order = order->next;
			
					if(rmorder->khach->sit==0){
						if(dltlist == nullptr){
							dltlist = rmorder->khach->cus;
						} else {
							dltlist->next = rmorder->khach->cus;
							rmorder->khach->cus->prev = dltlist;
							dltlist = rmorder->khach->cus;
						}
                        kickwait(rmorder->khach);
                        kickorder(rmorder);
                        }
                    else {
                        remove(rmorder->khach->cus);
						if(dltlist == nullptr){
							dltlist = rmorder->khach->cus;
						} else {
							dltlist->next = rmorder->khach->cus;
							rmorder->khach->cus->prev = dltlist;
							dltlist = rmorder->khach->cus;
						}
                        delete rmorder->khach;
                        kickorder(rmorder);
                    }
				 }
				 else order = order->next;
            }	
			while (dltlist !=nullptr){
				customer * dltcus = dltlist;
				dltlist->print();
				dltlist = dltlist->prev;
				delete dltcus;
			}
			int k = MAXSIZE - count;
			if(k > wait) k = wait;
			for(int i =0;i<k;i++){
				Node* waiter =headwait;
                outwait(waiter);
                waiter->sit=1;
				addcus(waiter->cus);
			}
		}

		void LIGHT(int num)
		{
			if(num>0){
            	customer* crcus=currentcus;
            	for(int i=0;i<count;i++) {
              	crcus->print();
              	crcus = crcus->next;  
            	}
			}
			else if(num<0){
				customer* crcus=currentcus;
            	for(int i=0;i<count;i++) {
              	crcus->print();
              	crcus = crcus->prev;  
            	}
			}
			else if(num ==0){
             	Node* crcus = headwait;
           		for(int i=0;i<wait;i++) {
             	crcus->cus->print();
              	crcus = crcus->next;  
            	}
			}
		

		}
};