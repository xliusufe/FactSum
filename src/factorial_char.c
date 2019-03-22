#include <stdio.h> 
#include <stdlib.h>
#include <string.h> 
#include <R.h>
#include <Rinternals.h>  

int factorial_small_sum(int *fact, int *len, int n){
	int i=2,fact0=1, sum=1,Ln;	
	while(i<=n) {
		fact0*=i++;
		sum+=fact0;
	}
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
	while(i<=n) {
		fact0*=i++;
	}
	fact[0] = fact0;
	i = fact0; Ln = 1;
	while(i/10){i = i/10; Ln++;	}
	len[0] = Ln-1;
	return 0;
}

int add(char *a, char *b, int curr_len, int s){
	//add b to a
	int i;
	int temp;
	for(i=0;i<curr_len;i++){
		temp = a[i+s]+b[i]; 
		if(temp<10) a[i+s]=temp;
		else {a[i+s]=temp-10; a[i+s+1]+=1;}
	}
	if(a[s+curr_len]>0)curr_len++;
	return curr_len+s;
}

int multiply_single(char *b, int curr_len, int n){
	//multiply b by n (n=0,1,2,...,9)
	int i,temp;
	int temp1=0;
	for(i=0;i<curr_len;i++){
		temp = b[i]*n+temp1; 
		b[i]=temp%10;
		temp1 = temp/10;
	}
	if(temp1>0){ b[curr_len]=temp1;curr_len++;}
	return curr_len;
}

int multiply(char *b, char *temp, char *temp1, int L, int curr_len0, int n){
	//multiply b by n
	int i,m=n,count,curr_len=curr_len0,curr_len1=curr_len0;

	for(i=0;i<L;i++) temp1[i]=b[i];
	if(m%10!=0) curr_len = multiply_single(b,curr_len0,m%10);
	else for(i=0;i<curr_len;i++) b[i]=0;
	m = (int)(m/10);
	count=1;
	while(m>0){
		for(i=0;i<curr_len;i++) temp[i]=temp1[i];
		if(m%10!=0){
			curr_len1 = multiply_single(temp,curr_len0,m%10);
			curr_len1 = add(b,temp,curr_len1,count);
		}
		curr_len=curr_len1;
		m = (int)(m/10);		
		count++;
	}	
	return curr_len;
}

int factorial_sum(char *b, char *fact_n, int *len, int Ln, int n){
	int i,curr_len=len[0];
	char *temp, *temp1;
	temp = (char*) malloc(sizeof(char)*len[2]);
	temp1 = (char*) malloc(sizeof(char)*len[2]);	

	i=len[3];
	while(i<=n){
		curr_len = multiply(fact_n,temp,temp1,len[2],curr_len,i++);
		len[0] = curr_len;		
		curr_len = add(b,fact_n,curr_len,0);
		len[1] = curr_len;
		if(curr_len>len[2]-Ln){
			len[3] = i;
			free(temp);free(temp1);
			return 0;
		}
	}
	len[3] = i;
	free(temp);free(temp1);
	return 1;
}

int factorial(char *fact_n, int *len, int Ln, int n){
	int i,curr_len=len[0];
	char *temp, *temp1;
	temp = (char*) malloc(sizeof(char)*len[2]);
	temp1 = (char*) malloc(sizeof(char)*len[2]);	

	i=len[3];
	while(i<=n){
		curr_len = multiply(fact_n,temp,temp1,len[2],curr_len,i++);
		len[0] = curr_len;		
		len[1] = curr_len;
		if(curr_len>len[2]-Ln){
			len[3] = i;
			free(temp);free(temp1);
			return 0;
		}
	}
	len[3] = i;
	free(temp);free(temp1);
	return 1;
}

SEXP fact(SEXP N)
{
	int *n_ = INTEGER(N), *len;
	int n = n_[0], i;	
	SEXP rlen,rfact_s,list, list_names; 

	PROTECT(rlen = allocVector(INTSXP, 5));
	len = INTEGER(rlen);
	for(i=0;i<5;i++) len[i] = 1;

	if(n<11){	
		int *fact_n= (int*)malloc(sizeof(int)*1);
		factorial_small(fact_n,len,n);
		char *fact_s =(char *)malloc(sizeof(char)*len[0]);
		itoa(fact_n[0],fact_s,10);	

		PROTECT(rfact_s = allocVector(STRSXP, 1));
		SET_STRING_ELT(rfact_s, 0,  mkChar(fact_s));
		free(fact_n);
	}
	else{	
		int j,L=4*n, curr_len, Ln;
		char *fact_n = (char *)malloc(sizeof(char)*L);

		len[2]=L; len[3]=2; len[4] = 1;
		fact_n[0]=1; 
		for(i=1;i<L;i++) fact_n[i]=0;
		i = n; Ln = 2;
		while(i/10){i = i/10; Ln++;	}

		int pout=0;
		while(!pout){
			pout=factorial(fact_n,len, Ln,n);
			if(!pout){
				char *f1;
				curr_len=len[2];
				len[2] = 2*len[2];
				f1 = (char*)realloc(fact_n,sizeof(char)*len[2]);
				if(!f1) error("Out of memery!"); 
				fact_n=f1;
				for(j=curr_len;j<len[2];j++)fact_n[j]=0;
			}	
		}		
		char *fact_s =(char *)malloc(sizeof(char)*(len[0]+1));
		for(i=0;i<len[0];i++) fact_s[i] = fact_n[len[0]-1-i]+48;	
		fact_s[len[0]] = '\0';

		PROTECT(rfact_s = allocVector(STRSXP, 1));
		SET_STRING_ELT(rfact_s, 0,  mkChar(fact_s));
		free(fact_n);
	}
	char *names[2] = {"fact", "len"};
	PROTECT(list_names = allocVector(STRSXP, 2));
	for(i = 0; i < 2; i++)
		SET_STRING_ELT(list_names, i,  mkChar(names[i]));
	PROTECT(list = allocVector(VECSXP, 2)); 
	SET_VECTOR_ELT(list, 0, rfact_s);
	SET_VECTOR_ELT(list, 1, rlen);  
	setAttrib(list, R_NamesSymbol, list_names); 

	UNPROTECT(4);  
	return(list);	
}

SEXP fact_sum(SEXP N)
{
	int *n_ = INTEGER(N), *len;
	int n = n_[0], i;	
	char *fact_s,*fact_sum;
	SEXP rlen,rfact_s, rfact_sum,list, list_names;

	PROTECT(rlen = allocVector(INTSXP, 5));
	len = INTEGER(rlen);
	for(i=0;i<5;i++) len[i] = 1;
	if(n<11){		
		int *fact_n= (int*)malloc(sizeof(int)*2);
		factorial_small_sum(fact_n,len,n);

		fact_s =(char *)malloc(sizeof(char)*len[0]);
		fact_sum =(char *)malloc(sizeof(char)*len[1]);
		itoa(fact_n[0],fact_s,10);
		itoa(fact_n[1],fact_sum,10);

		PROTECT(rfact_s = allocVector(STRSXP, 1));
		PROTECT(rfact_sum = allocVector(STRSXP, 1));
		SET_STRING_ELT(rfact_s, 0,  mkChar(fact_s));
		SET_STRING_ELT(rfact_sum, 0,  mkChar(fact_sum));
		free(fact_n);
	}
	else{	
		int j,L=4*n, curr_len, Ln;
		char *bb,*fact_n;
		fact_n = (char *)malloc(sizeof(char)*L);
		bb = (char*) malloc(sizeof(char)*L);

		len[2]=L; len[3]=2; len[4]=1;
		fact_n[0]=1; bb[0]=1;
		for(i=1;i<L;i++){ fact_n[i]=0; bb[i]=0;}
		i = n; Ln = 2;
		while(i/10){i = i/10; Ln++;	}

		int pout=0;
		while(!pout){
			pout=factorial_sum(bb,fact_n,len, Ln,n);
			if(!pout){
				char *b1,*f1;
				curr_len=len[2];
				len[2] = 2*len[2];
				b1 = (char*)realloc(bb,sizeof(char)*len[2]);
				f1 = (char*)realloc(fact_n,sizeof(char)*len[2]);
				if((!b1)|(!f1)){error("Out of memery!"); len[4]=0;}
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
		free(fact_n); free(bb);
	}
	char *names[3] = {"fact", "fuct_sum", "len"};
	PROTECT(list_names = allocVector(STRSXP, 3));
	for(i = 0; i < 3; i++)
		SET_STRING_ELT(list_names, i,  mkChar(names[i]));
	PROTECT(list = allocVector(VECSXP, 3)); 
	SET_VECTOR_ELT(list, 0, rfact_s);
	SET_VECTOR_ELT(list, 1, rfact_sum);
	SET_VECTOR_ELT(list, 2, rlen);  
	setAttrib(list, R_NamesSymbol, list_names); 

	UNPROTECT(5);  
	return(list);	
}
