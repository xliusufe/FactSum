# FactSum
R package "FactSum" Calculates the factorial of a large integer, which may be much greater than the maximum memery of any data type. FactSum implements dramatically fast. It takes only 2.57 seconds to cumpute 10000! (it approximates 2.8E35660), and 2.98 seconds to cupute 10000! and sum=1!+2!+3!+...+10000!.

# Installation

    #install.packages("devtools")
    library(devtools)
    install_github("xliusufe/FactSum")

# Usage

   - [x] [FactSum-manual.pdf](https://github.com/xliusufe/FactSum/blob/master/inst/FactSum-manual.pdf) ---------- Details of the usage of the package.
# Example
    library(FactSum)

    fit <- fact(20)
    fit <- fact(20,1)
    

# Development
This R package is developed by Xu Liu (liu.xu@sufe.edu.cn) .

