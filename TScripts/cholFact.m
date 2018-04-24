function[L] = cholFact(A)
%Cholesky Factorization;
	n = length(A(:, 1));
	L = A;
	for k = 1:n
		L(k, k) = sqrt(L(k, k));
		for i = (k + 1):n
			L(i, k) = L(i, k) / L(k, k);
		end
		for j = (k + 1):n
			for i = (k + 1):n
				L(i, j) = L(i, j) - L(i, k)*L(j, k);
			end
		end
	end
	for i = 1:(n - 1)
		for j = (i + 1):n
			L(i, j) = 0;
		end
	end
end
