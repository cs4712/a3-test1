#!/usr/bin/env python
import sys,os
import re
import math


class Point(object):
    def __init__ (self, x, y):
        self.x = float(x)
        self.y = float(y)
    def __str__ (self):
        return '(' + '{0:.2f}'.format(self.x) + ',' + '{0:.2f}'.format(self.y) + ')'


class Line(object):
    def __init__ (self, src, dst):
        self.src = src
        self.dst = dst
    
    def __str__(self):
        return str(self.src) + '-->' + str(self.dst)


def Intersect(l1, l2):      # intersect function 
    x1, y1 = l1.src.x, l1.src.y
    x2, y2 = l1.dst.x, l1.dst.y
    x3, y3 = l2.src.x, l2.src.y
    x4, y4 = l2.dst.x, l2.dst.y
    if (x1 == x2 and y1 ==y2) or (x3 == x4 and y3 == y4):     # no street
        xcoor = ''
    else:
        xnum = (x1*y2-y1*x2)*(x3-x4) - (x1-x2)*(x3*y4-y3*x4)
        xden = (x1-x2)*(y3-y4) - (y1-y2)*(x3-x4)
        ynum = (x1*y2 - y1*x2)*(y3-y4) - (y1-y2)*(x3*y4-y3*x4)
        yden = (x1-x2)*(y3-y4) - (y1-y2)*(x3-x4)

        if xden == 0 and xnum == 0 and ynum == 0:    # overlap
            mpdistance = math.sqrt( math.pow(((x1+x2-x3-x4) / 2), 2) + math.pow(((y1+y2-y3-y4) / 2), 2) )
            linedistance = ( math.sqrt( math.pow((x1-x2), 2) + math.pow((y1-y2), 2) ) + math.sqrt( math.pow((x3-x4), 2) + math.pow((y3-y4), 2) ) ) / 2  

            if mpdistance > linedistance:
                xcoor = ''
            elif mpdistance == linedistance:
                if (x3 == x1 and y3 == y1) or (x3 == x2 and y3 == y2):
                    xcoor = x3
                    ycoor = y3
                elif (x4 == x1 and y4 == y1) or (x4 == x2 and y4 == y2):
                    xcoor = x4
                    ycoor = y4
                else:
                    pass
            elif mpdistance < linedistance:
                if ( (x3 == x1 and y3 == y1) and (x4 == x2 and y4 == y2) ) or ( (x4 == x1 and y4 == y1) and (x3 == x2 and y3 == y2) ):
                    xcoor = ''
                else:
                    if x3 <= max(x1,x2) and x3 >= min(x1,x2) and y3 <= max(y1,y2) and y4 >= min(y1,y2):      # 1 intersection
                        xcoor = x3
                        ycoor = y3
                    if y4 <= max(x1,x2) and x4 >= min(x1,x2) and y4 <= max(y1,y2) and y4 >= min(y1,y2):     # 1 intersection
                        xcoor = x4
                        ycoor = y4
                    if ( x3 <= max(x1,x2) and x3 >= min(x1,x2) and y3 <= max(y1,y2) and y4 >= min(y1,y2) ) and ( y4 <= max(x1,x2) and x4 >= min(x1,x2) and y4 <= max(y1,y2) and y4 >= min(y1,y2) ):
                        return 1     # 2 intersections
                    else:
                        xcoor = ''
        elif xden == 0 and (xnum != 0 or ynum != 0):    # no intersection
            xcoor = ''
        else:               # one intersection
            xcoor =  xnum / xden
            ycoor = ynum / yden
            if (xcoor <= max(x1,x2)) and (xcoor >= min(x1,x2)) and (xcoor <= max(x3,x4)) and (xcoor >= min(x3,x4)) :
                pass
            else:
                xcoor = ''
            if (ycoor <= max(y1,y2)) and (ycoor >= min(y1,y2)) and (ycoor <= max(y3,y4)) and (ycoor >= min(y3,y4)) :
                pass
            else:
                xcoor = ''

    if xcoor != '':
        if xcoor == 0:
            xcoor = 0
        if ycoor == 0:
            ycoor = 0
        return Point(xcoor, ycoor)
    else:
        return 0


class ParseException(Exception):
    def __init__(self, value):
        self._value = value
    
    def __str__(self):
        return str(self._value)


def parse_line1(line):
    global streetname
    global coordinate
    if line.startswith('a'):
        r1_name = re.compile(r'[ ]["][a-zA-Z]+(?:[ ][a-zA-Z]+)*["]')
        name1_match = r1_name.findall(line)
        if name1_match :
            if len(streetname) == 0:
                pass
            else:
                for i in range(0, len(streetname)):
                    if str.lower(name1_match[0]) == str.lower(streetname[i]):
                        raise ParseException("repetitive street name")
            r = re.compile(r'(?:[ ]*\([ ]*[-]*\d+[ ]*,[ ]*[-]*\d+[ ]*\)){2,}')
            coor_match = r.findall(line)
            if coor_match :
                if len(coordinate) == 0:
                    pass
                else:
                    for i in range(0, len(coordinate)):
                        if coor_match[0] == coordinate[i]:
                            raise ParseException("repetitive street coordinate")
                streetname.append(name1_match[0])
                coordinate.append(coor_match[0])
            else :
                raise ParseException("invalid coordinate input")
        else :
            raise ParseException("invalid street name")
    elif line.startswith('c'):
        r2_name = re.compile(r'[ ]["][a-zA-Z]+(?:[ ][a-zA-Z]+)*["]')
        name2_match = r2_name.findall(line)
        if name2_match:
            i = 0
            j = 0
            while i < len(streetname):
                if str.lower(name2_match[0]) == str.lower(streetname[i]):
                    j += 1
                    r = re.compile(r'(?:[ ]*\([ ]*[-]*\d+[ ]*,[ ]*[-]*\d+[ ]*\)){2,}')
                    coor_match = r.findall(line)
                    if coor_match :
                        for i in range(0,len(streetname)):
                            if str.lower(name2_match[0]) == str.lower(streetname[i]):
                                coordinate[i] = coor_match[0]
                    else :
                        raise ParseException("invalid coordinate input")
                else :
                    pass
                i += 1
            if j == 0:
                raise ParseException("this street doesn't exist")
        else :
            raise ParseException("invalid street name")
    elif line.startswith('g'):
        Graph(coordinate)
    elif line.startswith('r'):
        r2_name = re.compile(r'[ ]["][a-zA-Z]+(?:[ ][a-zA-Z]+)*["]')
        name2_match = r2_name.findall(line)
        if name2_match:
            i = 0
            j = 0
            while i < len(streetname):
                if str.lower(name2_match[0]) == str.lower(streetname[i]):
                    j += 1
                    streetname.pop(i)
                    coordinate.pop(i)
                    break
                else:
                    pass
                i += 1
            if j == 0:
                raise ParseException("this street doesn't exist")
        else :
            raise ParseException("invalid street name")
    else:
        raise ParseException("wrong command")


def Graph(streets):
    lists = [[] for i in range(0,len(streets))]      #create a two-dimensional array to store street coordinate             

    if len(streets) > 0:
        for i  in range(0,len(streets)):                 # split and store coordinate information
            coor_split = re.split(r'[(,)\s]', streets[i])        
            coor1 = filter(lambda x:x !='' , coor_split)
            
            for s in range(0,len(coor1)):
                coor1[s] = int(coor1[s])

            x1 = []
            y1 = []
            for s in range(0,len(coor1)-1,2):
                x1.append(coor1[s])
                y1.append(coor1[s+1])
            
            for j in range(0,len(coor1)-2,2):       # create line format
                l = Line(Point(int(coor1[j]), int(coor1[j+1])), Point(int(coor1[j+2]), int(coor1[j+3])))
                lists[i].append(l)
        
        n = [[] for i in range(0,len(streets))]      # create a three-dimensional array to store intersect coordinate

        vertex = []
        vertex1 = []
        graph = []
        graphx = []
        graphy = []
        for i in range(0,len(lists)):               # get intersect
            for j in range(0,len(lists[i])):
                n[i].append([])                     # create a three-dimensional array to store intersect coordinate
                for k in range(0,len(lists)):
                    if k != i :
                        for t in range(0,len(lists[k])):
                            m = Intersect(lists[i][j],lists[k][t])
                            if m == 0:
                                pass
                            elif m == 1:
                                n[i][j].append(lists[k][t].src)
                                n[i][j].append(lists[k][t].dst)
                            else: 
                                n[i][j].append(m)      
                    else:
                        pass
                if len(n[i][j]) == 1:
                    E1 = Line(lists[i][j].src, n[i][j][0])
                    E2 = Line(lists[i][j].dst, n[i][j][0])
                    graph.append(E1)
                    graph.append(E2)
                    vertex.append(E1.src)
                    vertex.append(E1.dst)
                    vertex.append(E2.src)
                    vertex.append(E2.dst)
                elif len(n[i][j]) > 1:
                    for h1 in range(0,len(n[i][j])):
                        tem = n[i][j][h1]
                        for h2 in range(h1+1,len(n[i][j])):
                            if lists[i][j].src.x == lists[i][j].dst.x :
                                if lists[i][j].src.y < lists[i][j].dst.y :
                                    if n[i][j][h2].y <= n[i][j][h1].y :
                                        n[i][j][h1] = n[i][j][h2]
                                        n[i][j][h2] = tem
                                    else:
                                        pass
                                else:
                                    if n[i][j][h2].y >= n[i][j][h1].y :
                                        n[i][j][h1] = n[i][j][h2]
                                        n[i][j][h2] = tem
                                    else:
                                        pass
                            else:
                                if lists[i][j].src.x < lists[i][j].dst.x :
                                    if n[i][j][h2].x <= n[i][j][h1].x :
                                        n[i][j][h1] = n[i][j][h2]
                                        n[i][j][h2] = tem
                                    else:
                                        pass
                                else:
                                    if n[i][j][h2].x >= n[i][j][h1].x :
                                        n[i][j][h1] = n[i][j][h2]
                                        n[i][j][h2] = tem
                                    else:
                                        pass

                    E1 = Line(lists[i][j].src, n[i][j][0])
                    graph.append(E1)
                    vertex.append(E1.src)
                    vertex.append(E1.dst)
                    for h3 in range(0,len(n[i][j])-1):
                        E3 = Line(n[i][j][h3], n[i][j][h3+1])
                        graph.append(E3)
                        vertex.append(E3.src)
                        vertex.append(E3.dst)
                    E2 = Line(lists[i][j].dst, n[i][j][len(n[i][j])-1])
                    graph.append(E2)
                    vertex.append(E2.src)
                    vertex.append(E2.dst)
                else:
                    pass


        number = 0
    
        for i in vertex:
            if str(i) not in vertex1:
                vertex1.append(str(i))
                number += 1
        
        sys.stdout.write( 'V ' + str(number) + '\n' )
        sys.stdout.flush()           
        for i in range(0,len(graph)):
            for j in range(0,len(vertex1)):
                if str(graph[i].src) == str(vertex1[j]):
                    graphx.append(j)
                if str(graph[i].dst) == str(vertex1[j]):
                    graphy.append(j)


        first = 0
        sys.stdout.write( 'E {' )
        for i in range(0,len(graph)):
            num1 = 0
            for j in range(0, i):
                if (graphx[i] == graphy[j]) and (graphy[i] == graphx[j]):
                    num1 += 1
                    break
                if (graphx[i] == graphx[j]) and (graphy[i] == graphy[j]):
                    num1 += 1
                    break
            if graphx[i] != graphy[i] and num1 == 0:
                if first == 0:
                    sys.stdout.write( '<' + str(graphx[i]) + ',' + str(graphy[i]) + '>' )
                else:
                    sys.stdout.write( ',' + '<' + str(graphx[i]) + ',' + str(graphy[i]) + '>' )
                first = first + 1
            else:
                pass
        sys.stdout.write( '}\n' )
        sys.stdout.flush() 

    else:
        sys.stdout.write( 'V 0\n' )
        sys.stdout.flush() 
        sys.stdout.write( 'E {}\n' )
        sys.stdout.flush() 
    
    return 0


def main():
    
    while True:
        line = sys.stdin.readline()      # add a street
        if line == '':
            break
        else :
            print 'read a line:', line
            try :
                parse_line1(line)
            except ParseException as ex:
                print "Error: {0}\n".format(ex)

    sys.exit(0)

if __name__ == '__main__':
    streetname = []
    coordinate = []
    main()

