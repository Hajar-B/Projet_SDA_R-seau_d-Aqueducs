# Execution:
#   python visualisation.py

from matplotlib import pyplot
from numpy import loadtxt

def first_draw():
    # draw the cities
    cities = loadtxt("resuCities.dat", dtype=float, delimiter=" ")
    pyplot.scatter(cities[:,1], cities[:,2], s=cities[:,0]/1000, c=cities[:,0], alpha=0.5);

    # draw the graph
    graph = loadtxt("resuGraph.dat", dtype=int)
    print(graph.shape[0])
    for x in range(graph.shape[0]):
      edge = [graph[x,0], graph[x,1]]
      pyplot.plot(cities[edge,1], cities[edge,2], 'b')

    pyplot.xlabel('Longitude', size=16)
    pyplot.ylabel('Latitude', size=16)

    # save in a PNG file
    pyplot.savefig('result.png')

    # show the result
    pyplot.show()

def draw_min_spanning_tree():
    #input number of habitants
    population = str(input("Saisir le nombre d'habitant : "))
    
    cities = loadtxt("resuCities.dat", dtype=float, delimiter=" ")
    pyplot.scatter(cities[:,1], cities[:,2], s=cities[:,0]/1000, c=cities[:,0], alpha=0.5);

    # draw the graph
    graph = loadtxt("graphe"+population+".dat", dtype=int)
    print(graph.shape[0])
    for x in range(graph.shape[0]):
      edge = [graph[x,0], graph[x,1]]
      pyplot.plot(cities[edge,1], cities[edge,2], 'b')

    pyplot.xlabel('Longitude', size=16)
    pyplot.ylabel('Latitude', size=16)

    # save in a PNG file
    pyplot.savefig('optimal'+population+'.png')

    # show the result
    pyplot.show()

def main():
    first_draw();
    draw_min_spanning_tree();

if __name__ == '__main__':
    main()
