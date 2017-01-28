(fn myfn [a x]
    (apply list 
    (let [gooda (reverse (map-indexed (fn [ind v] [ind v]) a))
        n (count a)
        ans (vec (repeat x '()))]
        (reduce
        (fn [ansl pr]
            (let [ind (mod (nth pr 0) x)
                 key (nth pr 1)]
                 (assoc ansl ind 
                 (conj (nth ansl ind) key))))
        (into [ans] gooda)))))