#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <map>

#include <string>
#include <fstream>
#include <sstream>
#include <optional>
#include <source_location>

namespace Shader {
	class VertexShader;
	class FragmentShader;

	enum ShaderParameterType {
		Float, Int, Bool
	};

	struct ShaderParameter {
		ShaderParameterType type;
		bool is_set;
		ShaderParameter(): ShaderParameter(Float){};
		ShaderParameter(ShaderParameterType t): type(t), is_set(false){};
	};

	struct DebugInformation {
		bool m_first_used;
		std::map<const char*, ShaderParameter> m_parameters;
	};

	class MainShader {
	public:
		MainShader();
		bool isLinked() const;
		bool hasFragment() const;
		bool hasVertex() const;
		void addVertex(VertexShader& vs);
		void addFragment(FragmentShader& fs);
		void use();
		void link();

		void setInt(const char* name, int value) const;
		void setFloat(const char* name, float value) const;
		void setVec2(const char* name, glm::vec2 v) const;
		void setVec3(const char* name, glm::vec3 v) const;
		void setVec4(const char* name, glm::vec4 v) const;
		void setMat4(const char* name, glm::mat4 m) const;

	private:
		GLint getUniform(const char* name) const {
			return glGetUniformLocation(m_id, name);
		}
		bool setParamFound(const char* name);
		unsigned int m_id;
		bool m_is_linked;
		bool m_has_fragment;
		bool m_has_vertex;
		//bool m_is_debug;
		std::optional<DebugInformation> m_debug;
	};

	class ProdShader {
	public:
		ProdShader();

		void addVertex(VertexShader& vs);
		void addFragment(FragmentShader& fs);
		void use();

		bool setFloat(const char* name, float value);
	private:
		unsigned int m_id;
	};

	class DebugShader {
	public:
		DebugShader();

		void addVertex(VertexShader& vs);
		void addFragment(FragmentShader& fs);

		void use();
		void useDebug(const char* shader_name, std::source_location source);

		void logParamWarnings(const char* shader_name) const;

		bool link(); // to link the added shaders
		bool getIsLinked() const;

		bool setFloat(const char* name, float value);
		bool setVec2(const char* name, glm::vec2& v);
		bool setVec2(const char* name, float v1, float v2);
		bool setMat4(const char* name, const glm::mat4& mat);
	private:
		GLint getUniform(const char* name) {
			return glGetUniformLocation(m_id, name);
		}
		bool setParamFound(const char* name);

		unsigned int m_id;
		bool m_is_linked;
		bool m_first_used;
		std::map<const char*, ShaderParameter> m_parameters;
	};

	class ShaderComposition {
	public:
		ShaderComposition(unsigned int id);
		unsigned int getId() const {
			return m_id;
		}
		std::map<const char*, ShaderParameterType> getParameters() const {
			return m_parameters;
		}
		void addVariable(const char* name, ShaderParameterType var_type) {
			m_parameters.insert(std::make_pair(name, var_type));
		}

	protected:
		unsigned int m_id;
		std::map<const char*, ShaderParameterType> m_parameters;
	};

	class VertexShader : public ShaderComposition {
	public:
		VertexShader();
		bool addSource(const char* source);
	private:

	};


	class FragmentShader : public ShaderComposition {
	public:
		FragmentShader();
		bool addSource(const char* source);
	private:
	};

	std::optional<VertexShader> createVertexShaderFromFile(const char* path);
	std::optional<FragmentShader> createFragmentShaderFromFile(const char* path);
}