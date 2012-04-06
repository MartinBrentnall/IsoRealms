/*
 * Copyright 2009 Martin Brentnall
 *
 * This file is part of Iso-Realms.
 *
 * Iso-Realms is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Iso-Realms is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Iso-Realms.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "AttractDemo.h"

AttractDemo::AttractDemo() {
  init();
  cRotation = 0.0f;
  
  
  // Start Of User Initialization

//   glClearColor(0.0f, 0.0f, 0.0f, 0.5f);
//   glClearDepth(1.0f);
//   glDepthFunc(GL_LEQUAL);
//   glEnable(GL_DEPTH_TEST);
//   glShadeModel(GL_SMOOTH);
//   glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);



  glGenFramebuffersEXT(1, &cFrameBuffer);                         // create a new framebuffer
  glGenTextures(1, &cTexture);                         // and a new texture used as a color buffer

  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, cFrameBuffer);                 // switch to the new framebuffer

  glBindTexture(GL_TEXTURE_2D, cTexture);                    // Bind the colorbuffer texture
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);       // make it linear filterd
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 512, 512, 0,GL_RGBA, GL_INT, NULL);  // Create the texture data
  glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT,GL_COLOR_ATTACHMENT0_EXT,GL_TEXTURE_2D, cTexture, 0); // attach it to the framebuffer

  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
}

void AttractDemo::init() {
}

void AttractDemo::update(int ticks) {
  cRotation += 0.1 * ticks;
}

void AttractDemo::drawBox() {
  glBegin(GL_QUADS);
  // Front Face
  glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);  // Bottom Left Of The Texture and Quad
  glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);  // Bottom Right Of The Texture and Quad
  glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);  // Top Right Of The Texture and Quad
  glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);  // Top Left Of The Texture and Quad
  // Back Face
  glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);  // Bottom Right Of The Texture and Quad
  glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);  // Top Right Of The Texture and Quad
  glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);  // Top Left Of The Texture and Quad
  glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);  // Bottom Left Of The Texture and Quad
  // Top Face
  glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);  // Top Left Of The Texture and Quad
  glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f,  1.0f,  1.0f);  // Bottom Left Of The Texture and Quad
  glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f,  1.0f,  1.0f);  // Bottom Right Of The Texture and Quad
  glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);  // Top Right Of The Texture and Quad
  // Bottom Face
  glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);  // Top Right Of The Texture and Quad
  glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f, -1.0f, -1.0f);  // Top Left Of The Texture and Quad
  glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);  // Bottom Left Of The Texture and Quad
  glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);  // Bottom Right Of The Texture and Quad
  // Right face
  glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);  // Bottom Right Of The Texture and Quad
  glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);  // Top Right Of The Texture and Quad
  glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);  // Top Left Of The Texture and Quad
  glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);  // Bottom Left Of The Texture and Quad
  // Left Face
  glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);  // Bottom Left Of The Texture and Quad
  glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);  // Bottom Right Of The Texture and Quad
  glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);  // Top Right Of The Texture and Quad
  glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);  // Top Left Of The Texture and Quad
  glEnd();
}

void AttractDemo::render() {
  // FBO render pass
  glViewport (0, 0, 512, 512);
  glBindTexture(GL_TEXTURE_2D, 0);
  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, cFrameBuffer);
  glClearColor (1.0f, 0.0f, 0.0f, 0.5f);
  glClear(GL_COLOR_BUFFER_BIT);
  glLoadIdentity();
  glTranslatef(0.0f, 0.0f, -6.0f);
  glRotatef(cRotation,0.0f,1.0f,0.0f);
  glRotatef(cRotation,1.0f,0.0f,0.0f);
  glRotatef(cRotation,0.0f,0.0f,1.0f);
  glColor3f(1,1,0);
  drawBox();
  
  // Draw the CUBE!
  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
  glClearColor(0.0f, 0.0f, 0.0f, 0.5f);
  glClear(GL_COLOR_BUFFER_BIT);
  glBindTexture(GL_TEXTURE_2D, cTexture);
  glViewport(0, 0, 1024, 768);
  glLoadIdentity();
  glTranslatef(0.0f, 0.0f, -6.0f);
  glRotatef(cRotation,0.0f,1.0f,0.0f);
  glRotatef(cRotation,1.0f,0.0f,0.0f);
  glRotatef(cRotation,0.0f,0.0f,1.0f);
  glColor3f(1,1,1);
  drawBox();
  glFlush();
  
  
/*  glBindTexture(GL_TEXTURE_2D, 0);
  glLoadIdentity();
  glTranslatef(0.0f, 0.0f, -3.0f);
  glRotatef(cRotation, 0.0f, 0.0f, 1.0f);
  glBegin( GL_TRIANGLES );
  glColor3f(1.0f, 0.0f, 0.0f); glVertex3f( 0.0f,  1.0f, 0.0f);
  glColor3f(0.0f, 1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 0.0f);
  glColor3f(0.0f, 0.0f, 1.0f); glVertex3f( 1.0f, -1.0f, 0.0f);
  glEnd();*/
  glLoadIdentity();
}

bool AttractDemo::hasFinished() {
  return false;
}

void AttractDemo::frontEndActive(bool active) {
  // TODO: Implement this
}

extern "C" IAttract* create() {
  return new AttractDemo();
}

extern "C" void destroy(IAttract* attract) {
  delete attract;
}
