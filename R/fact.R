fact <- function(n,is.sum=FALSE){

  if(n<0) stop("n must be a non negative integer!")
  if(!is.sum){
    fitc <- .Call("fact", as.integer(n))
    if(!fitc$len[5]) printf("Out of memery!")
    fit <- list(fact=fitc$fact,
                len_fact=fitc$len[1],
                nzeros=fitc$len[3]
                )
  }
  else{
    fitc <- .Call("fact_sum", as.integer(n))
    if(!fitc$len[5]) printf("Out of memery!")
    fit <- list(fact=fitc$fact,
                len_fact=fitc$len[1],
                fact_sum=fitc$fact_sum,
                len_sum=fitc$len[2],
                nzeros=fitc$len[3]
                )
  }
  fit
}
