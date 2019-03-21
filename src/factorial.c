#include <stdio.h>  // required for exit
#include <stdlib.h> // required for malloc(), free();
#include <Rinternals.h>  // required for SEXP et.al.;
#include <string.h> // required for memcpy()
#include <time.h>

int factorial_small_sum(int *fact, int *fact_sum, int *len, int n){
	int i=2,fact0=1, sum=1,Ln;	
	while(i<=n) {
		fact0*=i++;
		sum+=fact0;
	}
	fact[0] = fact0; fact_sum[0] = sum;
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

int add(int *a, int *b, int curr_len, int s){
	//add b to a
	int i;
	int temp,temp1=0;
	for(i=0;i<curr_len;i++){
		temp = a[i+s]+b[i]+temp1; 
		a[i+s]=temp%10;
		temp1 = temp/10;
	}
	if(temp1>0){ a[s+curr_len]=temp1;curr_len++;}
	return curr_len+s;

}

int multiply_single(int *b, int curr_len, int n){
	//multiply b by n (n=0,1,2,...,9)
	int i,temp,temp1=0;
	for(i=0;i<curr_len;i++){
		temp = b[i]*n+temp1; 
		b[i]=temp%10;
		temp1 = (int)(temp/10);
	}
	if(temp1>0){ b[curr_len]=temp1;curr_len++;}
	return curr_len;
}

int multiply(int *b, int *temp, int *temp1, int L, int curr_len0, int n){
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

int factorial_sum(int *b, int *fact_n, int *len, int Ln, int n){
	int i,curr_len=len[0];
	int *temp, *temp1;
	temp = (int*) malloc(sizeof(int)*len[2]);
	temp1 = (int*) malloc(sizeof(int)*len[2]);	

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

int factorial(int *fact_n, int *len, int Ln, int n){
	int i,curr_len=len[0];
	int *temp, *temp1;
	temp = (int*) malloc(sizeof(int)*len[2]);
	temp1 = (int*) malloc(sizeof(int)*len[2]);	

	i=len[3];
	while(i<=n){
		curr_len = multiply(fact_n,temp,temp1,len[2],curr_len,i++);
		len[0] = curr_len;		
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
	int *fact_s;
	SEXP rlen,rfact_s,list, list_names;

	if(n<11){		
		PROTECT(rlen = allocVector(INTSXP, 5));
		len = INTEGER(rlen);
		for(i=0;i<5;i++) len[i] = 1;			

		PROTECT(rfact_s = allocVector(INTSXP, 1));
		fact_s = INTEGER(rfact_s);
		factorial_small(fact_s,len,n);
	}
	else{	
		int j,L=2*n, curr_len, Ln;
		
		PROTECT(rlen = allocVector(INTSXP, 5));
		len = INTEGER(rlen);

		int *fact_n;
		fact_n = (int *)malloc(sizeof(int)*L);

		len[0]=len[1]=1;len[2]=L; len[3]=2; len[4] = 1;
		fact_n[0]=1; 
		for(i=1;i<L;i++) fact_n[i]=0;
		i = n; Ln = 2;
		while(i/10){i = i/10; Ln++;	}

		int pout=0;
		while(pout==0){
			pout=factorial(fact_n,len, Ln,n);
			if(!pout){
				int *f1;
				curr_len=len[2];
				len[2] = 2*len[2];
				f1 = (int*)realloc(fact_n,sizeof(int)*len[2]);
				if(!f1){printf("Out of memery!"); len[4]=0;}
				fact_n=f1;
				for(j=curr_len;j<len[2];j++)fact_n[j]=0;
			}	
		}
		PROTECT(rfact_s = allocVector(INTSXP, len[0]));		
		fact_s = INTEGER(rfact_s);		
		for(i=0;i<len[0];i++) fact_s[i] = fact_n[len[0]-1-i];	
		free(fact_n);
	}

	char *names[2] = {"fact", "len"};
	PROTECT(list_names = allocVector(STRSXP, 2));
	for(i = 0; i < 2; i++)
		SET_STRING_ELT(list_names, i,  mkChar(names[i]));
	PROTECT(list = allocVector(VECSXP, 3)); 
	SET_VECTOR_ELT(list, 0, rfact_s);
	SET_VECTOR_ELT(list, 1, rlen);  
	setAttrib(list, R_NamesSymbol, list_names); 

	free(fact_s);
	UNPROTECT(4);  
	return(list);	
}

SEXP fact_sum(SEXP N)
{
	int *n_ = INTEGER(N), *len;
	int n = n_[0], i;	
	int *fact_s, *fact_sum;
	SEXP rlen,rfact_s, rfact_sum,list, list_names;

	if(n<11){		
		PROTECT(rlen = allocVector(INTSXP, 5));
		len = INTEGER(rlen);
		for(i=0;i<5;i++) len[i] = 1;			

		PROTECT(rfact_s = allocVector(INTSXP, 1));
		PROTECT(rfact_sum = allocVector(INTSXP, 1));
		fact_s = INTEGER(rfact_s);
		fact_sum = INTEGER(rfact_sum);
		factorial_small_sum(fact_s,fact_sum,len,n);
	}
	else{	
		int j,L=2*n, curr_len, Ln;
		
		PROTECT(rlen = allocVector(INTSXP, 5));
		len = INTEGER(rlen);

		int *bb,*fact_n;
		fact_n = (int *)malloc(sizeof(int)*L);
		bb = (int*) malloc(sizeof(int)*L);

		len[0]=len[1]=1;len[2]=L; len[3]=2; len[4]=1;
		fact_n[0]=1; bb[0]=1;
		for(i=1;i<L;i++){ fact_n[i]=0; bb[i]=0;}
		i = n; Ln = 2;
		while(i/10){i = i/10; Ln++;	}

		int pout=0;
		while(pout==0){
			pout=factorial_sum(bb,fact_n,len, Ln,n);
			if(!pout){
				int *b1,*f1;
				curr_len=len[2];
				len[2] = 2*len[2];
				b1 = (int*)realloc(bb,sizeof(int)*len[2]);
				f1 = (int*)realloc(fact_n,sizeof(int)*len[2]);
				if((!b1)|(!f1)){printf("Out of memery!"); len[4]=0;}
				bb=b1; fact_n=f1;
				for(j=curr_len;j<len[2];j++){ fact_n[j]=0; bb[j]=0;}
			}	
		}
		PROTECT(rfact_s = allocVector(INTSXP, len[0]));
		PROTECT(rfact_sum = allocVector(INTSXP, len[1]));
		fact_s = INTEGER(rfact_s);
		fact_sum = INTEGER(rfact_sum);
		for(i=0;i<len[0];i++) fact_s[i] = fact_n[len[0]-1-i];
		for(i=0;i<len[1];i++) fact_sum[i] = bb[len[1]-1-i];
		free(fact_n);free(bb);
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

	free(fact_s);free(fact_sum);
	UNPROTECT(5);  
	return(list);	
}
