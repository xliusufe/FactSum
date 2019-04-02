#include <stdio.h>  // required for exit
#include <stdlib.h> // required for malloc(), free();
#include <R.h>
#include <Rinternals.h>  // required for SEXP et.al.;
#include <string.h> // required for memcpy()

int int2char(int n,char* str){
	int i=n,j=1;
	char temp;
	str[0] = i%10+48;
	while(i/10){i = i/10; str[j++]=i%10+48;}
	for(i=0;i<j/2;i++){
		temp = str[i]; str[i]=str[j-i-1];str[j-i-1]=temp;
	}
	str[j] = '\0';
	return j;
}

int factorial_small_sum(int *fact, int *len, int n){
	int i=2,fact0=1, sum=1,Ln;	
	while(i<=n) {fact0*=i++; sum+=fact0;}
	fact[0] = fact0; fact[1] = sum;
	i = fact0; Ln = 1;
	while(i/10){i = i/10; Ln++;	}
	len[0] = Ln-1;
	
	i = sum; Ln = 1;
	while(i/10){i = i/10; Ln++;	}
	len[1] = Ln-1;

	return 0;
}

int factorial_small(int *fact, int *len, int n){
	int i=2,fact0=1,Ln;	
	while(i<=n) fact0*=i++;
	fact[0] = fact0;
	i = fact0; Ln = 1;
	while(i/10){i = i/10; Ln++;	}
	len[0] = Ln-1;
	return 0;
}

int plus(int *b, int *pf, int curr_len, int count5, int count10){
	int i,temp=0,*pa=b,*pb=pf;
	pa+=count5+count10; pb+=count10;
	for(i=0;i<curr_len;i++,pa++,pb++){
		temp = *pa+*pb+temp/10; 
		*pa=temp%10;
	}
	if(temp/10){*pa=temp/10;curr_len++;}
	return curr_len+count5+count10;
}

int product(int curr_len, int n, int *pf, int digits){
	int i,temp1=0,*pb=pf;
	curr_len+=digits-1;
	for(i=0;i<curr_len;i++,pb++){
		temp1=(*pb)*n+temp1/10; 
		*pb=temp1%10;
	}
	if(temp1/10){ *pb=temp1/10;curr_len++;}
	return curr_len;
}

int factorial_sum(int *b, int *fact_n, int *len, int Ln, int n){
	int i=2,curr_len=0,curr_len1=0,digits=2,up=100,fact0=1,sum=1,count5=2,count10=0;
	int k,*pf=fact_n+2;
	while(i<=10) {fact0*=i++; sum+=fact0;}
	i = fact0;fact_n[0]=i%10;
	while(i/10){i = i/10;fact_n[++curr_len]=i%10;}
	i = sum;b[0]=i%10;
	while(i/10){i = i/10;b[++curr_len1]=i%10;}
	curr_len-=2; 
	i=11;
	while(i<=n){	
		k=i;
		while(!(k/10)){k/=10;count10++;}
		curr_len = product(curr_len,k,pf,digits);
		curr_len1 = plus(b,pf,curr_len,count5,count10);
		if(curr_len1>len[2]-Ln){
			len[3] = i;
			return 0;
		}
		i++;
		if(i==up){ digits++; up*=10;}
		while(!(*pf)){pf++;curr_len--;count5++;}
	}
	len[0] = curr_len+count5+count10; len[1] = curr_len1;	
	len[3] = i; len[2] = count5+count10;
	return 1;
}

int factorial(int *fact_n, int *len, int Ln, int n){
	int i=2,curr_len=0,curr_len1=0,digits=2,up=100,fact0=1,count5=2,count10=0;
	int k,*pf=fact_n+2;
	while(i<=10)fact0*=i++;
	i = fact0;fact_n[0]=i%10;
	while(i/10){i = i/10;fact_n[++curr_len]=i%10;}
	curr_len-=2; 
	i=11;
	while(i<=n){	
		k=i;
		while(!(k/10)){k/=10;count10++;}
		curr_len = product(curr_len,k,pf,digits);
		if(curr_len>len[2]-Ln){
			len[3] = i;
			return 0;
		}
		i++;
		if(i==up){ digits++; up*=10;}
		while(!(*pf)){pf++;curr_len--;count5++;}
	}
	len[0] = curr_len+count5+count10;
	len[3] = i; len[2] = count5+count10;
	return 1;
}

SEXP fact(SEXP N)
{
	int *n_ = INTEGER(N);
	int n = n_[0], i,*len, *fact_n;
	char *fact_s;
	SEXP rlen,rfact_s,list, list_names;
	
	PROTECT(rlen = allocVector(INTSXP, 5));
	len = INTEGER(rlen);
	for(i=0;i<5;i++) len[i] = 1;

	if(n<11){		
		fact_n= (int*)malloc(sizeof(int)*1);
		factorial_small(fact_n,len,n);
		fact_s =(char *)malloc(sizeof(char)*(len[0]+1));
		int2char(fact_n[0],fact_s);
		if(n<=5) len[2]=0;
		else len[2]=1;
		if(n==10) len[2]=2;
		PROTECT(rfact_s = allocVector(STRSXP, 1));
		SET_STRING_ELT(rfact_s, 0,  mkChar(fact_s));
	}
	else{	
		int j,L, curr_len, Ln=1;
		i = n;
		while(i/10){i = i/10; Ln++;	}
		L=(Ln-1+i*0.1)*n;
		fact_n = (int *)malloc(sizeof(int)*L);
		len[2]=L; len[3]=2; len[4] = 1;
		fact_n[0]=1; 
		for(i=1;i<L;i++) fact_n[i]=0;


		int pout=0;
		while(!pout){
			pout=factorial(fact_n,len, Ln,n);
			if(!pout){
				int *f1;
				curr_len=len[2];
				len[2] = 2*len[2];
				f1 = (int*)realloc(fact_n,sizeof(int)*len[2]);
				if(!f1){error("Out of memory!"); len[4]=0;}
				fact_n=f1;
				for(j=curr_len;j<len[2];j++)fact_n[j]=0;
			}	
		}
		fact_s =(char *)malloc(sizeof(char)*(len[0]+1));
		for(i=0;i<len[0];i++) fact_s[i] = fact_n[len[0]-1-i]+48;	
		fact_s[len[0]] = '\0';
		PROTECT(rfact_s = allocVector(STRSXP, 1));
		SET_STRING_ELT(rfact_s, 0,  mkChar(fact_s));		
	}
	char *names[2] = {"fact", "len"};
	PROTECT(list_names = allocVector(STRSXP, 2));
	for(i = 0; i < 2; i++)
		SET_STRING_ELT(list_names, i,  mkChar(names[i]));
	PROTECT(list = allocVector(VECSXP, 2)); 
	SET_VECTOR_ELT(list, 0, rfact_s);
	SET_VECTOR_ELT(list, 1, rlen);  
	setAttrib(list, R_NamesSymbol, list_names); 
		
	free(fact_n);
	UNPROTECT(4);  
	return(list);	
}

SEXP fact_sum(SEXP N)
{
	int *n_ = INTEGER(N);
	int n = n_[0], i, *len,*fact_n;	
	char *fact_s, *fact_sum;
	SEXP rlen,rfact_s, rfact_sum,list, list_names;
	
	PROTECT(rlen = allocVector(INTSXP, 5));
	len = INTEGER(rlen);
	for(i=0;i<5;i++) len[i] = 1;

	if(n<11){
		fact_n= (int*)malloc(sizeof(int)*2);
		factorial_small_sum(fact_n,len,n);
		
		fact_s =(char *)malloc(sizeof(char)*(len[0]+1));
		fact_sum =(char *)malloc(sizeof(char)*(len[1]+1));
		int2char(fact_n[0],fact_s);
		int2char(fact_n[1],fact_sum);
		if(n<=5) len[2]=0;
		else len[2]=1;
		if(n==10) len[2]=2;
		PROTECT(rfact_s = allocVector(STRSXP, 1));
		PROTECT(rfact_sum = allocVector(STRSXP, 1));
		SET_STRING_ELT(rfact_s, 0,  mkChar(fact_s));
		SET_STRING_ELT(rfact_sum, 0,  mkChar(fact_sum));		
	}
	else{	
		int j,L, curr_len, Ln=1;
		i = n;
		while(i/10){i = i/10; Ln++;	}
		L=(Ln-1+i*0.1)*n;
		fact_n = (int *)malloc(sizeof(int)*L);
		int *bb = (int*) malloc(sizeof(int)*L);

		len[2]=L; len[3]=2; len[4]=1;
		fact_n[0]=1; bb[0]=1;
		for(i=1;i<L;i++){ fact_n[i]=0; bb[i]=0;}

		int pout=0;
		while(!pout){
			pout=factorial_sum(bb,fact_n,len, Ln,n);
			if(!pout){
				int *b1,*f1;
				curr_len=len[2];
				len[2] = 2*len[2];
				b1 = (int*)realloc(bb,sizeof(int)*len[2]);
				f1 = (int*)realloc(fact_n,sizeof(int)*len[2]);
				if((!b1)|(!f1)){error("Out of memory!"); len[4]=0;}
				bb=b1; fact_n=f1;
				for(j=curr_len;j<len[2];j++){ fact_n[j]=0; bb[j]=0;}
			}	
		}
		fact_s =(char *)malloc(sizeof(char)*(len[0]+1));
		fact_sum =(char *)malloc(sizeof(char)*(len[1])+1);
		for(i=0;i<len[0];i++) fact_s[i] = fact_n[len[0]-1-i]+48;
		for(i=0;i<len[1];i++) fact_sum[i] = bb[len[1]-1-i]+48;
		fact_s[len[0]] = '\0'; fact_sum[len[1]] = '\0';

		PROTECT(rfact_s = allocVector(STRSXP, 1));
		PROTECT(rfact_sum = allocVector(STRSXP, 1));
		SET_STRING_ELT(rfact_s, 0,  mkChar(fact_s));
		SET_STRING_ELT(rfact_sum, 0,  mkChar(fact_sum));
		free(bb);
	}

	char *names[3] = {"fact", "fact_sum", "len"};
	PROTECT(list_names = allocVector(STRSXP, 3));
	for(i = 0; i < 3; i++)
		SET_STRING_ELT(list_names, i,  mkChar(names[i]));
	PROTECT(list = allocVector(VECSXP, 3)); 
	SET_VECTOR_ELT(list, 0, rfact_s);
	SET_VECTOR_ELT(list, 1, rfact_sum);
	SET_VECTOR_ELT(list, 2, rlen);  
	setAttrib(list, R_NamesSymbol, list_names); 
		
	free(fact_n);
	UNPROTECT(5); 
	return(list);	
}
