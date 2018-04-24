function[A, b, x] = backSub(A, b)
%Backward Substitution; solves for Ax = b when A is upper triangular
	n = length(b);
	for f = 1:n
		x(f) = 0;
	end
	x(n) = b(n) / A(n, n);
	for i = (n - 1):-1:1
		soln = b(i);
		for j = (i + 1):n
			soln = soln - A(i, j)*x(j);
		end
		x(i) = soln / A(i, i);
	end
end	
