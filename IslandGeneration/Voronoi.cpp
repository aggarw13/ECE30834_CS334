/*
 * Voronoi.cpp
 *
 *  Created on: Nov 23, 2015
 *      Author: aimanuslim
 */

#include <iostream>
#include "GL/glut.h"
#include <cstdlib>
#include <ctime>
#include <math.h>
#include "IslandGeneration.h"

void adjustVoronoi(double * voronoiPositions, double * voronoiColors, double elevation[windowWidth][windowHeight], double * landColor, int idx){
	int x = 0, y = 0, i = 0;
	double currHeight;
	while(i < idx){
		x = (int) ( ( (double) (voronoiPositions[i] + 1.0f) / 2.0f)     * windowWidth);
		y = (int) ( ( (double) (voronoiPositions[i + 1] + 1.0f) / 2.0f) * windowHeight);

		if(
			(x > 0 && x < windowWidth) &&
			(y > 0 && y < windowHeight)
		){
			currHeight = elevation[x][y];
			//z-value
			voronoiPositions[i + 2] = -currHeight;

			// r
			voronoiColors[i] = landColor[x + y * windowWidth * 3];
			// g
			voronoiColors[i + 1] = landColor[x + 1 + y * windowWidth * 3];
			// b
			voronoiColors[i + 2] = landColor[x + 2 + y * windowWidth * 3];

		}

		i += 3;

	}

}

int VoronoiVerticesColors(VD vd, double * voronoiPoints, double * voronoiColors){
	// Fill in points for voronoi edges
	VD::Face_iterator ft = vd.faces_begin();
	//	DT dt = vd.dual();
	//	DT::Vertex_iterator dv_iter = dt.vertices_begin();

	int	i = 0, ict=0, x, y, idx = 0;
	double currHeight;
	// specify voronoi surfaces
	for(;ft != vd.faces_end();ft++){
	//	for(;ict<5;ft++, ict++){
		if(!(*ft).is_unbounded()){
			VD::Delaunay_vertex_handle vh = (*ft).dual();
			Point_2 center = vh->point();
			double currX = center.x();
			double currY = center.y();
			Ccb_halfedge_circulator et_start = (*ft).ccb();
			Ccb_halfedge_circulator et = et_start;

			do {
				if(et->has_source() && et->has_target()){

					// middle
	//					delaunayPoints[didx++] = currX;
	//					delaunayPoints[didx++] = currY;
	//					delaunayPoints[didx++] = ZPOS - 0.4;
	//					delaunayColors[didx++] = 1.0f; //(double) ((rand() % (int) 255) + 1) / 255.0f;//
	//					delaunayColors[didx++] = 0.0f; //(double) ((rand() % (int) 255) + 1) / 255.0f;// 0.0f;
	//					delaunayColors[didx++] = 0.0f; //(double) ((rand() % (int) 255) + 1) / 255.0f;//0.0f;
					if(
							(currX > -1 && currX < 1) && (currY > -1) && (currY < 1) &&
							(et->source()->point().x() > -1) && (et->source()->point().x() < 1) &&
							(et->source()->point().y() > -1) && (et->source()->point().y() < 1)

					){

							voronoiPoints[idx++] = currX;
							voronoiPoints[idx++] = currY;
							voronoiPoints[idx++] = ZPOS + 0.2;


							voronoiColors[i++] = 1.0f; //(double) ((rand() % (int) 255) + 1) / 255.0f;//
							voronoiColors[i++] = 0.0f; //(double) ((rand() % (int) 255) + 1) / 255.0f;// 0.0f;
							voronoiColors[i++] = 0.0f; //(double) ((rand() % (int) 255) + 1) / 255.0f;//0.0f;

							// first vertex of triangle
							voronoiPoints[idx++] = et->source()->point().x();
							voronoiPoints[idx++] = et->source()->point().y();
			//					voronoiPoints[idx++] = 0;
							voronoiPoints[idx++] = ZPOS + 0.2;
							voronoiColors[i++] = 0.0f; //(double) ((rand() % (int) 255) + 1) / 255.0f;//
							voronoiColors[i++] = 0.6f; //(double) ((rand() % (int) 255) + 1) / 255.0f;// 0.0f;
							voronoiColors[i++] = 1.0f; //(double) ((rand() % (int) 255) + 1) / 255.0f;//0.0f;
			//
							// second vertex of triangle
							voronoiPoints[idx++] = et->target()->point().x();
							voronoiPoints[idx++] = et->target()->point().y();
			//				voronoiPoints[idx++] = 0;
							voronoiPoints[idx++] = ZPOS + 0.2;
							voronoiColors[i++] = 0.0f; //(double) ((rand() % (int) 255) + 1) / 255.0f;//
							voronoiColors[i++] = 0.6f; //(double) ((rand() % (int) 255) + 1) / 255.0f;// 0.0f;
							voronoiColors[i++] = 1.0f; //(double) ((rand() % (int) 255) + 1) / 255.0f;//0.0f;
					}

				}

			} while ( ++et != et_start );

		}
	}
	return idx;
}





VD generateVoronoi(double ** setPoints, int numberofPoints){
	VD vd;

	int i = 0;
	for(i = 0; i < numberofPoints * 3; i+=3){
		Site_2 p(Point_2((*setPoints)[i], (*setPoints)[i + 1]));
		vd.insert(p);
	}

	return vd;

}
