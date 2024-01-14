" Define function to calculate determinant
function! Determinant(matrix)
    let size = len(a:matrix)
    if size == 1
        return a:matrix[0][0]
    elseif size == 2
        return a:matrix[0][0] * a:matrix[1][1] - a:matrix[0][1] * a:matrix[1][0]
    else
        let det = 0
        for j in range(size)
            let sub_matrix = []
            for i in range(1, size)
                let row = []
                for k in range(size)
                    if k != j
                        call add(row, a:matrix[i][k])
                    endif
                endfor
                call add(sub_matrix, row)
            endfor
            let sign = (-1) ** j
            let cofactor = sign * Determinant(sub_matrix)
            let det = det + a:matrix[0][j] * cofactor
        endfor
        return det
    endif
endfunction

" Define function to transpose matrix
function! Transpose(matrix)
    let result = []
    for j in range(len(a:matrix[0]))
        let row = []
        for i in range(len(a:matrix))
            call add(row, a:matrix[i][j])
        endfor
        call add(result, row)
    endfor
    return result
endfunction

" Define function to multiply matrix by scalar
function! ScalarMultiply(matrix, scalar)
    let result = []
    for row in a:matrix
        let new_row = []
        for element in row
            let product = a:scalar * element
            call add(new_row, product)
        endfor
        call add(result, new_row)
    endfor
    return result
endfunction

" Define function to add matrices
function! AddMatrices(matrix1, matrix2)
    let result = []
    for i in range(len(a:matrix1))
        let row = []
        for j in range(len(a:matrix1[0]))
            let sum = a:matrix1[i][j] + a:matrix2[i][j]
            call add(row, sum)
        endfor
        call add(result, row)
    endfor
    return result
endfunction

" Define function to subtract matrices
function! SubtractMatrices(matrix1, matrix2)
    let result = []
    for i in range(len(a:matrix1))
        let row = []
        for j in range(len(a:matrix1[0]))
            let difference = a:matrix1[i][j] - a:matrix2[i][j]
            call add(row, difference)
        endfor
        call add(result, row)
    endfor
    return result
endfunction

" Define function to multiply matrices
function! MultiplyMatrices(matrix1, matrix2)
    let result = []
    for i in range(len(a:matrix1))
        let row = []
        for j in range(len(a:matrix2[0]))
            let sum = 0
            for k in range(len(a:matrix1[0]))
                let product = a:matrix1[i][k] * a:matrix2[k][j]
                let sum = sum + product
            endfor
            call add(row, sum)
        endfor
        call add(result, row)
    endfor
    return result
endfunction

" Test the functions
let matrix1 = [[1, 2], [3, 4]]
let matrix2 = [[5, 6], [7, 8]]
let matrix3 = [[1 + i, 2 - 3i], [4i, 5 + 6i]]
let scalar = 2
echo Determinant(matrix1)
echo Transpose(matrix1)
echo ScalarMultiply(matrix1, scalar)
echo AddMatrices(matrix1, matrix2)
echo SubtractMatrices(matrix1, matrix2)
echo MultiplyMatrices(matrix1, matrix2)
echo MultiplyMatrices(matrix1, matrix3)
