\name{fact}
\alias{fact}
%- Also NEED an '\alias' for EACH other topic documented here.
\title{
Calculate the factorial of a large integer.
}
\description{
Calculate the factorial of a large integer, which mey be much greater than the maximum memory of any data type. FactSum implements dramatically fast. It takes only \eqn{2.57} seconds to cumpute \eqn{10000!} (it approximates \eqn{2.8E+35660}), and \eqn{2.98} seconds to compute \eqn{10000!} and \code{sum=1!+2!+3!+...+10000!} simultaneously.
}
\usage{
fact(n,is.sum=FALSE)
}
%- maybe also 'usage' for other objects documented here.
\arguments{
  \item{n}{
A non negative integer.
}
  \item{is.sum}{
Logical indicating  that fact out sum of all factorial, that is \eqn{\sum_{i=1}^{n}i!}, if is.sum=TRUE, and not if is.sum=FALSE. Default is FALSE.
}
}

\value{
  \item{fact}{The factorial of \eqn{n}, which is a string.}
  \item{len_fact}{The digit  of factorial of \eqn{n}, which is a integer.}
  \item{fact_sum}{The sumation of factorial of \eqn{n},  that is \eqn{\sum_{i=1}^{n}i!}, if is.sum=TRUE, which is a string.}
  \item{len_fact}{The digit of \eqn{\sum_{i=1}^{n}i!}, which is a integer.}
}

\author{
Xu Liu
}

\examples{
fit <- fact(10)
fit_sum <- fact(20,1)
}

