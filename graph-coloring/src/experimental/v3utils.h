#ifndef V3UTILSH
#define V3UTILSH

/* https://stackoverflow.com/questions/15843525/how-do-you-insert-the-value-in-a-sorted-vector */
template< typename T >
typename std::vector<T>::iterator 
   insert_sorted( std::vector<T> & vec, T const& item )
{
    return vec.insert
        ( 
            std::upper_bound( vec.begin(), vec.end(), item ),
            item 
        );
}



/* https://stackoverflow.com/questions/26719144/how-to-erase-a-value-efficiently-from-a-sorted-vector/26720032 */
void remove_sorted(std::vector<int> &vec, int value)
{
    auto pr = std::equal_range(std::begin(vec), std::end(vec), value);
    vec.erase(pr.first, pr.second);
}

int getNextCandidateColor(const std::vector<int> & v, int previous, int end) {

    for (int i = previous + 1; i < end; i++) {
        /* If invalidatedColors do not contain the color, it may be used */
        if (!binary_search(v.begin(), v.end(), i)) {
            return i;
        }
    }

    return -1;
}

std::vector< std::vector<int> > initColorsInvalidated(grafo::Grafo & G) {
    std::vector< std::vector<int> > colorsTaken(G.n);
    for (int i = 0; i < G.n; i++) {
        std::vector<int> n;
        colorsTaken[i] = n;
    }
    return colorsTaken;
}

#endif
