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

void int2charArry(int *p, int len, int r, char* str){
	int i,j,k,*temp1, temp, *p1=&p[len-1];
	char *p2=str;
	temp1 = (int *) malloc(sizeof(int)*r);
	temp = *p1--; j=0;
	while (temp/10){
		temp1[j++] = temp%10;
        temp = temp/10;
    }
	temp1[j] = temp;
	for (k=0;k<=j;k++)  *p2++ = temp1[j-k]+48;
    for(i=0;i<len-1;i++){
        for (j=0; j<r; j++) temp1[j]=0;
		temp = *p1;   j = 0;
        while (temp/10) {
            temp1[j++] = temp%10;
            temp = temp/10;
        }
        temp1[j] = temp;
		for (k=0;k<r;k++)  *p2++ = temp1[r-1-k]+48;
		p1--;
    }
    *p2 = '\0';
	free(temp1);
}

int powint(int a, int n){
	int i,b=1;
	for(i=0;i<n;i++) b*=a;
	return b;
}

int lengthint(int n){
	int L=1,temp=n;
	while(temp/10){ temp/=10; L++;}
	return L;
}

int zerosint(int n){
	int L=0,temp=n;
	while(!(temp%10)){ temp/=10; L++;}
	return L;
}

int factorial_small_sum(int *fact, int *len, int n){
	int i=2,fact0=1, sum=1;	
	while(i<=n) {fact0*=i++; sum+=fact0;}
	fact[0] = fact0; fact[1] = sum;
	len[0] = lengthint(fact0)-1;
	len[1] = lengthint(sum)-1;
	return 0;
}

int factorial_small(int *fact, int *len, int n){
	int i=2,fact0=1;	
	while(i<=n) fact0*=i++;
	fact[0] = fact0;
	len[0] = lengthint(fact0)-1;
	return 0;
}

int plus_radix(int *b, int *fact_n, int *len, int k, int radix){
    int curr_len=len[3]+1;
    int *pb=b+len[3],*pf=fact_n+len[3];
    int quot,temp=0;
    if (k==1)  return curr_len;
    else{
        while (curr_len<=len[2]) {
            quot=(*pb+*pf+temp)/radix;
            *pb=(*pb+*pf+temp)%radix;
            temp=quot;  pb++;
            pf++;  curr_len++;
            if ((*pb)==0 && (*pf)==0 && quot==0 && curr_len>len[0]) break;
        }
        if (curr_len>len[2])  return 0;
        else{
            curr_len--;
            return curr_len;
        }
    }
}

int product_radix(int *fact_n, int *len, int k, int radix){
    int curr_len=len[3]+1,s=0;
    int *pf=fact_n+len[3],*pl=len+3;
    int quot,temp=0;
    while (curr_len<=len[2]) {
        quot=((*pf)*k+temp)/radix;
        *pf=((*pf)*k+temp)%radix;
        temp=quot;
        if ((*pf)==0) {
            s++;
            if (len[3]+s==curr_len)  *pl = (*pl)+1;
        }
        pf++;  curr_len++;
        if (quot==0 && curr_len>len[0]) break;
    }
    if (curr_len>len[2])  return 0;
    else{
        curr_len--;
        return curr_len;
    }
}

int changeRadix(int *p, int len, int *l, int r, int r0){
    int *ptemp, j, k, t1, t2, m, L, Len=0,*p1,*p2;
    ptemp = (int *) malloc(sizeof(int)*2*len);
    p2=ptemp; for (j=0; j<2*len; j++) *p2++ = 0;
    if (r>1) {
		L = lengthint(p[len-1]);
        if (((len-1)*r0 + L)%r==0)  Len = ((len-1)*r0 + L)/r;
        else   Len = ((len-1)*r0 + L)/r + 1;
        t1 = powint(10, r);
        ptemp[0] = p[0]%t1;
        j = p[0]/t1; L = 1;  m = 0;
        for (k=1; ; k++) {
            t1 = powint(10, r-L);
            t2 = powint(10, L);
            ptemp[k] = j + p[k-m]%t1*t2;
            j = p[k-m]/t1; L++;
            if (j==0 && k>=Len) break;
            if (L==r) {
                ptemp[k+1] = j;
                t1 = pow(10.0, 1.0*r);
                ptemp[k+2] = p[k+1-m]%t1;
                j = p[k+1-m]/t1; L = 1;
                if (j==0 && (k+2)>=Len) break;
                k+=2;  m++;
            }
        }
    }else{
		p1=p; p2=ptemp;
        for (m=0; m<len; m++,p1++) {
            *p2++ = (*p1)%10; *p2++ = (*p1)/10;}
		Len = 2*(len-1)+lengthint(p[len-1]);
    }
	p1=p; p2=ptemp;
    for (k=0; k<Len; k++) *p1++ = *p2++;
	free(ptemp);
    return Len;
}

int factorial_radix(int *fact_n, int *len, int n){
    int i=1, radix, r0=5, r=r0, L, up=10000;
	radix=powint(10,r0); 
    while (i<=n) {		
        if (i==up&&r>1) {
			r--;
			L = zerosint(fact_n[len[3]]);
            len[3] = (len[3]*r0 + L)/r;
            len[0] = changeRadix(fact_n, len[0], len, r, r0);			
			radix/=10;	up*=10;	r0=r;
        }
        len[0] = product_radix(fact_n,len,i++,radix);
        if (len[0]==0 || len[1]==0) return 0;
    }
	int *p1=&fact_n[len[3]];L=0;
	while(!(*p1)){ L+=r; p1++;}			
	L += zerosint(*p1);
	len[3] = len[3]*r+L;
    return r;
}

int factorial_radix_sum(int *b, int *fact_n, int *len, int n){
    int i=1, radix, r0=5, r=r0, L, up=10000;
	radix=powint(10,r0); 
    while (i<=n) {		
        if (i==up&&r>1){
			r--;
			L = zerosint(fact_n[len[3]]);
            len[3] = (len[3]*r0 + L)/r;
            len[0] = changeRadix(fact_n, len[0], len, r, r0);			
            len[1] = changeRadix(b, len[1], len, r, r0);
			radix/=10;	up*=10;	r0=r;
        }
        len[0] = product_radix(fact_n,len,i,radix);
        len[1] = plus_radix(b,fact_n,len,i++,radix);
        if (len[0]==0 || len[1]==0) return 0;
    }
	int *p1=&fact_n[len[3]];L=0;
	while(!(*p1)){ L+=r; p1++;}
	L += zerosint(*p1);
	len[3] = len[3]*r+L;
    return r;
}

SEXP fact(SEXP N)
{
	int *n_ = INTEGER(N);
	int n = n_[0], i,*len, *fact_n,r;
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
		len[2] = (n<=5)?0:1;
		if(n==10) len[2]=2;
		PROTECT(rfact_s = allocVector(STRSXP, 1));
		SET_STRING_ELT(rfact_s, 0,  mkChar(fact_s));
	}
	else{	
		int j,L,curr_len, Ln=1;
		i = n;
		while(i/10){ i = i/10; Ln++;}
		r = (Ln<4)?6:(9-Ln);
		L=(Ln-1+i*0.5)*n;
		fact_n = (int *)malloc(sizeof(int)*L);
		len[2]=L; len[3]=0;
		fact_n[0]=1; 
		for(i=1;i<L;i++) fact_n[i]=0;


		int pout=0;
		while(!pout){
			pout=factorial_radix(fact_n,len,n);
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
		r = pout;
		L = lengthint(fact_n[len[0]-1])-r;
		fact_s =(char *)malloc(sizeof(char)*(len[0]*r+L+1));
		int2charArry(fact_n,len[0],r,fact_s);
		len[0] = len[0]*r+L;		
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
	int n = n_[0], i, *len,*fact_n,r;	
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
		len[2] = (n<=5)?0:1;
		if(n==10) len[2]=2;
		PROTECT(rfact_s = allocVector(STRSXP, 1));
		PROTECT(rfact_sum = allocVector(STRSXP, 1));
		SET_STRING_ELT(rfact_s, 0,  mkChar(fact_s));
		SET_STRING_ELT(rfact_sum, 0,  mkChar(fact_sum));		
	}
	else{	
		int j,L,curr_len, Ln=1;
		i = n;
		while(i/10){i = i/10; Ln++;	}
		L=(Ln-1+i*0.5)*n;
		r = (Ln<4)?6:(9-Ln);
		fact_n = (int *)malloc(sizeof(int)*L);
		int *bb = (int*) malloc(sizeof(int)*L);

		len[2]=L; len[3]=0;
		fact_n[0]=1; bb[0]=1;
		for(i=1;i<L;i++){ fact_n[i]=0; bb[i]=0;}

		int pout=0;
		while(!pout){
			pout=factorial_radix_sum(bb,fact_n,len,n);
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
		r = pout;
		L = lengthint(fact_n[len[0]-1])-r;
		fact_s =(char *)malloc(sizeof(char)*(len[0]*r+L+1));
		int2charArry(fact_n,len[0],r,fact_s);
		len[0] = len[0]*r+L;
		L = lengthint(bb[len[1]-1])-r;
		fact_sum =(char *)malloc(sizeof(char)*(len[1]*r+L+1));		
		int2charArry(bb,len[1],r,fact_sum);
		len[1] = len[1]*r+L;
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
