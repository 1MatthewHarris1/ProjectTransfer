function[A, b] = nGaussElimination(A, b)
%Naive Gauss Elimination; creates an upper triangular matrix	
	n = length(b);
	for k = 1:(n - 1)
		for i = (k + 1):n
			xMult = A(i, k)/A(k, k);
			A(i, k) = xMult;
			for j = (k + 1):n
				A(i, j) = A(i, j) - xMult*A(k, j);
			end
			b(i) = b(i) - xMult*b(k);
		end
	end
end
