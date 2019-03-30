# FactSum
R package "FactSum" Calculates the factorial of a large integer, which may be much greater than the maximum memory of any data type. FactSum implements dramatically fast. It takes only 2.57 seconds to cumpute 10000! (it approximates 2.8E35660), and 2.98 seconds to compute 10000! and sum=1!+2!+3!+...+10000! simultaneously.

# Installation

    #install.packages("devtools")
    library(devtools)
    install_github("xliusufe/FactSum")

# Usage

   - [x] [FactSum-manual.pdf](https://github.com/xliusufe/FactSum/blob/master/inst/FactSum-manual.pdf) ---------- Details of the usage of the package.
# Example
    library(FactSum)

    fit <- fact(10)
    print(fit$fact,quote=FALSE)
    fit$len_fact
    #------------------------
    fit_sum <- fact(20,1)
    print(fit_sum$fact,quote=FALSE)
    fit_sum$len_fact
    print(fit_sum$fact_sum,quote=FALSE)
    fit_sum$len_sum
    

# Development
This R package is developed by Xu Liu (liu.xu@sufe.edu.cn).

