# FactSum
R package "FactSum" Calculates the factorial of a large integer, which may be much greater than the maximum memory of any data type. FactSum implements dramatically fast. It takes only 0.45 seconds to cumpute 10000! (it approximates 2.8E+35660), and 0.98 seconds to compute 10000! and sum=1!+2!+3!+...+10000! simultaneously. It takes only one minute to cumpute 100000! (it approximates 2.8E+456574), and less then two minutes to compute 100000! and sum=1!+2!+3!+...+100000! simultaneously.

# Installation

    #install.packages("devtools")
    library(devtools)
    install_github("xliusufe/FactSum")

# Usage

   - [x] [FactSum-manual.pdf](https://github.com/xliusufe/FactSum/blob/master/inst/FactSum-manual.pdf) ---------- Details of the usage of the package.
   
   - [x] [factorial.md](https://github.com/xliusufe/FactSum/blob/master/inst/factorial.md) -------------------- The factorial of n=20, 50, 100, 1000, 10000, and 100000.
   
   - [x] [Web-based calculator](https://xliusufe.shinyapps.io/factorial/) --------- A web-based calculator 

   - [X] ["factpy"](https://github.com/xliusufe/factpy/) ------------------------ A Python package named  is available  
   
# Example
    library(FactSum)

    fit <- fact(10)
    print(fit$fact,quote=FALSE)
    fit$len_fact
    fit$nzeros
    #------------------------
    fit_sum <- fact(20,1)
    print(fit_sum$fact,quote=FALSE)
    fit_sum$len_fact
    fit_sum$nzeros
    print(fit_sum$fact_sum,quote=FALSE)
    fit_sum$len_sum
    fit_sum$nzeros
    

# Development
This R package is developed by Xu Liu (liu.xu@sufe.edu.cn).