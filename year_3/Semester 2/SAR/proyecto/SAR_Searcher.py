# version 1.1


import argparse

from SAR_lib import SAR_Indexer

if __name__ == "__main__":


    parser = argparse.ArgumentParser(description='Search the index.')

    parser.add_argument('index', metavar='index', type=str,
                        help='name of the index.')


    parser.add_argument('-C', '--count', dest='count', action='store_true', default=False, 
                    help='show only the number of documents retrieved.')


    parser.add_argument('-A', '--all', dest='all', action='store_true', default=False, 
                    help='show all the results. If not used, only the first 10 results are showed. Does not apply with -C and -T options.')
    

    out_group = parser.add_argument_group("Batch modes")
    group1 = out_group.add_mutually_exclusive_group()
    group1.add_argument('-Q', '--query', dest='query', metavar= 'query', type=str, action='store', help='query.')
    group1.add_argument('-L', '--list', dest='qlist', metavar= 'qlist', type=str, action='store', help='file with queries.')
    group1.add_argument('-T', '--test', dest='test', metavar= 'test', type=str, action='store', help='file with queries and results, for testing.')

    sem_group = parser.add_argument_group("Semantic Search")
    group0 = sem_group.add_mutually_exclusive_group()
    group0.add_argument('-S', '--semantic_threshold', dest= 'sthreshold', type=float, action='store', default=None, help='threshold for the semantic search.')
    group0.add_argument('-R', '--semantic_ranking', dest= 'sranking', action='store_true', help='active the semantic ranking of the binary search.')
    

    args = parser.parse_args()

    searcher = SAR_Indexer()
    searcher.load_info(args.index)
    searcher.set_showall(args.all)
    searcher.set_semantic_threshold(args.sthreshold)
    searcher.set_semantic_ranking(args.sranking)
    
    # se debe contar o mostrar resultados?
    if args.count is True:
        fnc = searcher.solve_and_count
    else:
        fnc = searcher.solve_and_show

    if args.qlist is not None:
        # opt: -L, una lista de queries
        with open(args.qlist, encoding='utf-8') as fh:
            query_list = fh.read().strip().split('\n')
        searcher.solve_and_count(query_list)

    elif args.test is not None:
        # opt: -T, testing
        with open(args.test, encoding='utf-8') as fh:
            query_list = fh.read().split('\n')
        if searcher.solve_and_test(query_list):
            print('\nParece que todo está bien, buen trabajo!')
        else:
            print('\nParece que hay alguna consulta mal :-(')            

    elif args.query is not None:
        # opt: -Q, una query pasada como argumento
        # se debe contar o mostrar resultados?
        if args.count is True:
            searcher.solve_and_count([args.query])
        else:
            searcher.solve_and_show(args.query)

    else:
        # modo interactivo
        query = input("query: ")
        while query != "":
            # se debe contar o mostrar resultados?
            if args.count is True:
                searcher.solve_and_count([query])
            else:
                searcher.solve_and_show(query)
            query = input("query: ")
