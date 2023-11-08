#ifndef FRAMEBUFFER_HPP
#define FRAMEBUFFER_HPP

class FBO{
    public:

        FBO(){
        }

        ~FBO(){
        }

        void CreateDepthMapFBO(unsigned int width, unsigned int height){
            m_width = width;
            m_height = height;
            // configure depth map FBO
            // -----------------------
            glGenFramebuffers(1, &m_fbo);

            // create depth texture
            glGenTextures(1, &m_textureID);
            glBindTexture(GL_TEXTURE_2D, m_textureID);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

            float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
            glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
            // attach depth texture as FBO's depth buffer
            glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_textureID, 0);
            glDrawBuffer(GL_NONE);
            glReadBuffer(GL_NONE);
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
        }

        void BindTexture(int slot){
            glActiveTexture(GL_TEXTURE0+slot);
            glBindTexture(GL_TEXTURE_2D, m_textureID);
        }

        void Bind(){

            glViewport(0, 0, m_width, m_height);
            glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
        }

        void Unbind(){
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
        }

    private:

        unsigned int m_textureID;
        unsigned int m_fbo;
        unsigned int m_width,m_height; // width and height of the texture.

};


#endif
