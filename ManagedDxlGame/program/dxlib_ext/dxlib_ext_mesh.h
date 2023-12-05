#pragma once
#include "../dxlib_ext/dxlib_ext.h"
#include "dxlib_ext_texture.h"
#include "dxlib_ext_render_param.h"
#include "dxlib_ext_mesh_desc.h"

namespace dxe {


	class MeshVbo final{
	public :
		~MeshVbo() {
			if (vb_hdl_) DeleteVertexBuffer(vb_hdl_);
			if (ib_hdl_) DeleteIndexBuffer(ib_hdl_);
		}
		int vb_hdl_ = 0;
		int ib_hdl_ = 0;
	};

	class Camera;
	class Mesh final {
	public :
		enum class eMeshFormat {
			MESH_FMT_PG,	// プログラマブルメッシュ
			MESH_FMT_MV		// DxLib 用 MV メッシュ
		};
		enum class eShapeType {
			NONE
			,PLANE
			,BOX
			,SPHERE
			,TRIANGLE_ISOSCELES
			,TRIANGLE_RIGHT
			,TRIANGLE_EQUILATERAL
			,DOME
			,CONE
			,CYLINDER
			,CONE_CYLINDER
			,DISK
			,DISK_RING
			,HOLLOW_OUT_DISK_PLANE
			,BARREL
			,HALF_BARREL
			,TORUS
			,HEIGHT_MAP
			,RESOUCE
		};
		enum eTextureSlot {
			DIFFUSE
			, AMIBENT
			, SPECULAR
			, BUMP
			, HEIGHT
		};
		enum class fDebugLine {
			TNL_ENUM_BIT_FLAG(FLG_NONE),
			TNL_ENUM_BIT_FLAG(FLG_AXIS),
			TNL_ENUM_BIT_FLAG(FLG_OBB),			
			TNL_ENUM_BIT_FLAG(FLG_AABB),
			TNL_ENUM_BIT_FLAG(FLG_BD_SPHERE),
			TNL_ENUM_BIT_FLAG(FLG_VTX_NML) // pg mesh only
		};
	public :
		~Mesh() {
			if (mv_hdl_) MV1DeleteModel(mv_hdl_);
		}

		fDebugLine		flg_dbg_line_ = fDebugLine::FLG_NONE;
		tnl::Vector3	pos_;
		tnl::Vector3	scl_ = { 1,1,1 };
		tnl::Quaternion rot_;


		//==========================================================================================================================
		// 
		//
		// function
		//
		//

		void copySetting( Mesh* other );

		//------------------------------------------------------------------------------------------------------------------------
		// クローンの生成
		Mesh* createClone();

		//-----------------------------------------------------------------------------------------------------
		// Create*** で生成したプログラマブルメッシュを Xファイルフォーマットでリソースとして保存する
		// arg1... 保存先のパス
		// ret.... [ true : 保存成功 ] [ false : 保存失敗 ]
		// tips... この機能が必要になった経緯
		//         DxLib の機能として MV*** 関数の中には DxLib が対応しているフォーマットの
		//         モデルデータをロードして使用しないと機能しない物があるため 例.シャドウマップ, サンプラステート　ブレンドステート等
		bool exportForFileFormatX(const std::string& file_path);

		//-----------------------------------------------------------------------------------------------------
		// Create*** で生成したプログラマブルメッシュを MVファイルフォーマットでリソースとして保存する
		// arg1... 保存先のパス
		// ret.... [ true : 保存成功 ] [ false : 保存失敗 ]
		// tips... MV はバイナリ形式なのでロードの高速化措置として用意
		bool exportForFileFormatMV(const std::string& file_path);


		//-----------------------------------------------------------------------------------------------------
		// 描画
		// arg1... カメラ
		void render(const Camera* camera);


		//==========================================================================================================================
		// 
		//
		// inline function
		//
		//

		int getDxMvHdl() { return mv_hdl_; }

		// テクスチャの設定
		inline void setTexture(std::shared_ptr<Texture> texture, uint32_t slot_num = DIFFUSE) {
			if (textures_.size() <= slot_num) {
				std::vector<std::shared_ptr<Texture>> new_buffer(slot_num+1);
				memcpy(new_buffer.data(), textures_.data(), sizeof(std::shared_ptr<Texture>) * textures_.size() );
				new_buffer[slot_num] = texture;
				textures_ = std::move(new_buffer);
			}
			else {
				textures_[slot_num] = texture;
			}
			if (eMeshFormat::MESH_FMT_MV == mesh_format_) MV1SetTextureGraphHandle(mv_hdl_, slot_num, texture->getDxLibGraphHandle(), true);
		}
		inline std::shared_ptr<Texture> getTexture(uint32_t slot_num = DIFFUSE) {
			if (textures_.empty()) return nullptr;
			return textures_[slot_num];
		}
		inline uint32_t getTextureSlotNum() { return (uint32_t)textures_.size(); }

		inline void setAlpha(const float alpha) { alpha_ = alpha; }
		inline float getAlpha() const { return alpha_; }

		inline void setName(const std::string& name) noexcept { mesh_name_ = name; }
		inline const std::string& getName() const noexcept { return mesh_name_; }

		// カリングモード設定
		// arg1... DX_CULLING_***
		inline void setCullingMode(const int mode) noexcept { render_param_.setCullingMode( mode ); }
		inline int getCullingMode() const noexcept { return render_param_.getCullingMode(); }

		// ブレンドモード設定
		// arg1... DX_BLENDMODE_***
		// tips... MV メッシュのみ対応 ( dxlib の仕様 )
		inline void setBlendMode(const int mode) noexcept { render_param_.setBlendMode(mode); }
		inline int getBlendMode() const noexcept { return render_param_.getBlendMode(); }

		// テクスチャサンプラフィルタ設定
		// arg1... DX_DRAWMODE_***
		// tips... MV メッシュのみ対応 ( dxlib の仕様 )
		inline void setSampleFilterMode(const int mode) noexcept { render_param_.setSampleFilterMode(mode); }
		inline int getSampleFilterMode() const noexcept { return render_param_.getSampleFilterMode(); }

		// デフォルトライトの有効化
		inline void setDefaultLightEnable(const bool f) noexcept { render_param_.setDefaultLightEnable(f); }
		inline int getDefaultLightEnable() const noexcept { return render_param_.getDefaultLightEnable(); }

		// 深度テストの有効化
		inline void setDepthTestEnable(const bool f) noexcept { render_param_.setDepthTestEnable(f); }
		inline bool getDepthTestEnable() const noexcept { return render_param_.getDepthTestEnable(); }

		// 深度バッファへの書き込み有効化
		inline void setWriteDepthBufferEnable(const bool f) noexcept { render_param_.setWriteDepthBufferEnable(f); }
		inline bool getWriteDepthBufferEnable() const noexcept { return render_param_.getWriteDepthBufferEnable(); }

		// アンビエントカラー設定
		inline void setMtrlAmibent(const tnl::Vector3& col) noexcept {
			render_param_.setMtrlAmbient(col);
			if (mesh_format_ == eMeshFormat::MESH_FMT_MV) MV1SetMaterialAmbColor(mv_hdl_, 0, render_param_.dxlib_mtrl_.Ambient);
		}
		inline tnl::Vector3 getMtrlAmbient() const noexcept { return render_param_.getMtrlAmbient(); }

		// ディフューズカラー設定
		inline void setMtrlDiffuse(const tnl::Vector3& col) noexcept {
			render_param_.setMtrlDiffuse(col);
			if (mesh_format_ == eMeshFormat::MESH_FMT_MV) MV1SetMaterialDifColor(mv_hdl_, 0, render_param_.dxlib_mtrl_.Diffuse);
		}
		inline tnl::Vector3 getMtrlDiffuse() const noexcept { return render_param_.getMtrlDiffuse(); }

		// スペキュラカラー設定
		inline void setMtrlSpecular(const tnl::Vector3& col) noexcept {
			render_param_.setMtrlSpecular(col);
			if (mesh_format_ == eMeshFormat::MESH_FMT_MV) MV1SetMaterialSpcColor(mv_hdl_, 0, render_param_.dxlib_mtrl_.Specular);
		}
		inline tnl::Vector3 getMtrlSpecular() const noexcept { return render_param_.getMtrlSpecular(); }

		// エミッシヴカラー設定
		inline void setMtrlEmissive(const tnl::Vector3& col) noexcept {
			render_param_.setMtrlEmissive(col);
			if (mesh_format_ == eMeshFormat::MESH_FMT_MV) MV1SetMaterialEmiColor(mv_hdl_, 0, render_param_.dxlib_mtrl_.Emissive);
		}
		inline tnl::Vector3 getMtrlEmissive() const noexcept { return render_param_.getMtrlEmissive(); }

		// スペキュラパワー設定
		inline void setMtrlSpecPower(const float pow) noexcept {
			render_param_.setMtrlSpecPower( pow );
			if (mesh_format_ == eMeshFormat::MESH_FMT_MV) MV1SetMaterialSpcPower(mv_hdl_, 0, render_param_.dxlib_mtrl_.Power);
		}
		inline float getMtrlSpecPower() const noexcept { return render_param_.getMtrlSpecPower(); }


		// 一括設定
		inline void setRenderParam( const RenderParam& param ) {
			render_param_ = param;
			if (mesh_format_ != eMeshFormat::MESH_FMT_MV) return;
			MV1SetMaterialAmbColor(mv_hdl_, 0, render_param_.dxlib_mtrl_.Ambient);
			MV1SetMaterialDifColor(mv_hdl_, 0, render_param_.dxlib_mtrl_.Diffuse);
			MV1SetMaterialSpcColor(mv_hdl_, 0, render_param_.dxlib_mtrl_.Specular);
			MV1SetMaterialEmiColor(mv_hdl_, 0, render_param_.dxlib_mtrl_.Emissive);
			MV1SetMaterialSpcPower(mv_hdl_, 0, render_param_.dxlib_mtrl_.Power);
		}

		// インデックスの取得
		inline const std::vector<uint32_t>& getIndexs() { return idxs_; }

		// 頂点座標の取得
		// tips... [idx[0]] [idx[1]] [idx[2]] で三角形１枚の頂点座標
		inline const std::vector<VERTEX3D>& getVertexs() { return vtxs_; }

		// 生成情報の取得
		inline const std::shared_ptr<MeshDesc> getCreateDesc() { return desc_; }

		// 形状の取得
		inline eShapeType getShapeType() { return shape_type_; }

		// ワールド行列でトランスフォームしたワールド空間頂点座標の取得
		// tips... [0][1][2] で三角形１枚の頂点座標
		std::vector<tnl::Vector3> createWorldVertexs();


		//==========================================================================================================================
		//
		// 
		// static function
		// 
		// tips... 幾何学形状の Create *** MV 関数で MV が付いているものに関して
		//         内部的に生成されるメッシュは MV 対応になりますが、生成時に一時的に
		// 　　　　ｘファイルフォーマットで保存をしてから再ロードを行っているので生成には相応のコストが掛かります。
		// 
		//

		//-----------------------------------------------------------------------------------------------------
		// MV 形式のメッシュテーブルを MVファイルの集合として保存する
		// arg1... 保存先のパス
		// arg2... MVメッシュテーブル
		// ret.... [ true : 保存成功 ] [ false : 保存失敗 ]
		// tips... ロードの高速化措置として用意
		static bool ExportForFileFormatMVT(const std::string& file_path, std::vector<Mesh*>& mesh_tbl);

		//------------------------------------------------------------------------------------------------------------------------
		// 四角形
		// arg1... 幅・高さ ( z 不使用 )  
		// arg2... 横分割数
		// arg3... 縦分割数
		// arg4... 左回りインデックス( 法線反転用 )
		static Mesh* CreatePlane( const tnl::Vector3& size_wh, const int div_w = 10, const int div_h = 10, const bool is_left_cycle = true, const tnl::Vector3& ltop_uv = {0, 0, 0}, const tnl::Vector3& rbottom_uv = {1, 1, 0}) noexcept;
		static Mesh* CreatePlaneMV(const tnl::Vector3& size_wh, const int div_w = 10, const int div_h = 10, const bool is_left_cycle = true, const tnl::Vector3& ltop_uv = { 0, 0, 0 }, const tnl::Vector3& rbottom_uv = { 1, 1, 0 }) noexcept;

		//------------------------------------------------------------------------------------------------------------------------
		// 立方体
		// arg1... サイズ
		// arg2... 横分割数
		// arg3... 縦分割数
		// arg4... 左回りインデックス( 法線反転用 )
		// tips... UV は各面へのテクスチャが横並びになっている事を想定した作りになっています。
		//         正面・右・奥・左・上・下の横並びになっているテクスチャを用意してください
		static Mesh* CreateBox(const float size, const int div_w = 5, const int div_h = 5, const bool is_left_cycle = true) noexcept;
		static Mesh* CreateBoxMV(const float size, const int div_w = 5, const int div_h = 5, const bool is_left_cycle = true) noexcept;

		//------------------------------------------------------------------------------------------------------------------------
		// 球
		// arg1... 半径
		// arg2... 横分割数
		// arg3... 縦分割数
		// arg4... 左回りインデックス( 法線反転用 )
		static Mesh* CreateSphere( const float radius, const int div_w = 10, const int div_h = 10, const bool is_left_cycle = true) noexcept;
		static Mesh* CreateSphereMV(const float radius, const int div_w = 10, const int div_h = 10, const bool is_left_cycle = true) noexcept;

		//------------------------------------------------------------------------------------------------------------------------
		// 二等辺三角形
		// arg1... 幅・高さ ( z 不使用 )  
		// arg2... 横分割数
		// arg3... 縦分割数
		// arg4... 左回りインデックス( 法線反転用 )
		static Mesh* CreateTriangleIsosceles(const tnl::Vector3& size_wh, const int div_w = 10, const int div_h = 10, const bool is_left_cycle = true) noexcept;
		static Mesh* CreateTriangleIsoscelesMV(const tnl::Vector3& size_wh, const int div_w = 10, const int div_h = 10, const bool is_left_cycle = true) noexcept;

		//-----------------------------------------------------------------------------------------------------
		// 直角三角形
		// arg1... 幅・高さ ( z 不使用 )  
		// arg2... 横分割数
		// arg3... 縦分割数
		// arg4... 左回りインデックス( 法線反転用 )
		static Mesh* CreateTriangleRight(const tnl::Vector3& size_wh, const int div_w = 10, const int div_h = 10, const bool is_left_cycle = true) noexcept;
		static Mesh* CreateTriangleRightMV(const tnl::Vector3& size_wh, const int div_w = 10, const int div_h = 10, const bool is_left_cycle = true) noexcept;

		//-----------------------------------------------------------------------------------------------------
		// 正三角形
		// arg1... 1 辺の長さ
		// arg2... 横分割数
		// arg3... 縦分割数
		// arg4... 左回りインデックス( 法線反転用 )
		static Mesh* CreateTriangleEquilateral(const float edge_lenght, const int div_w = 10, const int div_h = 10, const bool is_left_cycle = true) noexcept;
		static Mesh* CreateTriangleEquilateralMV(const float edge_lenght, const int div_w = 10, const int div_h = 10, const bool is_left_cycle = true) noexcept;

		//-----------------------------------------------------------------------------------------------------
		// ドーム
		// arg1... 半径
		// arg2... 横分割数
		// arg3... 縦分割数
		// arg4... 360 度 で完全な半球
		// arg5... 左回りインデックス( 法線反転用 )
		static Mesh* CreateDome(const float radius, const int div_w = 10, const int div_h = 10, const float angle = 360, const bool is_left_cycle = true) noexcept;
		static Mesh* CreateDomeMV(const float radius, const int div_w = 10, const int div_h = 10, const float angle = 360, const bool is_left_cycle = true) noexcept;

		//-----------------------------------------------------------------------------------------------------
		// 底面なしの円錐
		// arg1... 半径
		// arg2... 高さ
		// arg3... 横分割数
		// arg4... 縦分割数
		// arg5... 360 度 で底面が完全な円
		// arg6... 左回りインデックス( 法線反転用 )
		static Mesh* CreateCone(const float radius, const float height, const int div_w = 10, const int div_h = 10, const float angle = 360, const bool is_left_cycle = true) noexcept;
		static Mesh* CreateConeMV(const float radius, const float height, const int div_w = 10, const int div_h = 10, const float angle = 360, const bool is_left_cycle = true) noexcept;

		//-----------------------------------------------------------------------------------------------------
		// 上下に穴の空いた円錐
		// arg1... 上部半径
		// arg2... 下部半径
		// arg3... 高さ
		// arg4... 横分割数
		// arg5... 縦分割数
		// arg6... 360 度 で底面が完全な円
		// arg7... 左回りインデックス( 法線反転用 )
		static Mesh* CreateConeCylinder(const float top_radius, const float bottom_radius, const float height, const int div_w = 10, const int div_h = 10, const float angle = 360, const bool is_left_cycle = true) noexcept;
		static Mesh* CreateConeCylinderMV(const float top_radius, const float bottom_radius, const float height, const int div_w = 10, const int div_h = 10, const float angle = 360, const bool is_left_cycle = true) noexcept;

		//-----------------------------------------------------------------------------------------------------
		// 円盤
		// arg1... 半径
		// arg2... 横分割数
		// arg3... 縦分割数
		// arg4... 360 度 で完全な円
		// arg5... 左回りインデックス( 法線反転用 )
		// tips... 360度 180度 90度 のみ
		static Mesh* CreateDisk(const float radius, const int div_w = 10, const int div_h = 10, const int angle = 360, const bool is_left_cycle = true) noexcept;
		static Mesh* CreateDiskMV(const float radius, const int div_w = 10, const int div_h = 10, const int angle = 360, const bool is_left_cycle = true) noexcept;

		//-----------------------------------------------------------------------------------------------------
		// 円盤状のリング 
		// arg1... 半径
		// arg2... リングの太さ
		// arg2... 横分割数
		// arg3... 縦分割数
		// arg4... 360 度 で完全な円
		// arg5... 左回りインデックス( 法線反転用 )
		static Mesh* CreateDiskRing(const float radius, const float thickness, const int div_w = 10, const int div_h = 10, const float angle = 360, const bool is_left_cycle = true) noexcept;
		static Mesh* CreateDiskRingMV(const float radius, const float thickness, const int div_w = 10, const int div_h = 10, const float angle = 360, const bool is_left_cycle = true) noexcept;

		//-----------------------------------------------------------------------------------------------------
		// 円状にくり抜いた四角形
		// arg1... 半径
		// arg2... 横分割数
		// arg3... 縦分割数
		// arg4... 360 度 で円がくり抜かれた四角形
		// arg5... 左回りインデックス( 法線反転用 )
		static Mesh* CreateHollowOutDiskPlane(const float radius, const int div_w = 10, const int div_h = 10, const float angle = 360, const bool is_left_cycle = true) noexcept;
		static Mesh* CreateHollowOutDiskPlaneMV(const float radius, const int div_w = 10, const int div_h = 10, const float angle = 360, const bool is_left_cycle = true) noexcept;

		//-----------------------------------------------------------------------------------------------------
		// 円筒
		// arg1... 半径
		// arg2... 高さ
		// arg3... 横分割数
		// arg4... 縦分割数
		// arg5... 360 度 で円
		// arg6... 左回りインデックス( 法線反転用 )
		static Mesh* CreateCylinder(const float radius, const float height, const int div_w = 10, const int div_h = 10, const float angle = 360, const bool is_left_cycle = true) noexcept;
		static Mesh* CreateCylinderMV(const float radius, const float height, const int div_w = 10, const int div_h = 10, const float angle = 360, const bool is_left_cycle = true) noexcept;

		//-----------------------------------------------------------------------------------------------------
		// 樽
		// arg1... 上下の半径
		// arg2... 高さ
		// arg3... 上下の半径に対する膨らみ率
		// arg3... 横分割数
		// arg4... 縦分割数
		// arg5... 360 度 で円
		// arg6... 左回りインデックス( 法線反転用 )
		static Mesh* CreateBarrel(const float radius, const float height, const float bulge, const int div_w = 10, const int div_h = 10, const float angle = 360, const bool is_left_cycle = true) noexcept;
		static Mesh* CreateBarrelMV(const float radius, const float height, const float bulge, const int div_w = 10, const int div_h = 10, const float angle = 360, const bool is_left_cycle = true) noexcept;

		//-----------------------------------------------------------------------------------------------------
		// 上半分の樽
		// arg1... 上の半径
		// arg2... 高さ
		// arg3... 上の半径に対する膨らみ率
		// arg3... 横分割数
		// arg4... 縦分割数
		// arg5... 360 度 で円
		// arg6... 左回りインデックス( 法線反転用 )
		static Mesh* CreateHalfBarrel(const float radius, const float height, const float bulge, const int div_w = 10, const int div_h = 10, const float angle = 360, const bool is_left_cycle = true) noexcept;
		static Mesh* CreateHalfBarrelMV(const float radius, const float height, const float bulge, const int div_w = 10, const int div_h = 10, const float angle = 360, const bool is_left_cycle = true) noexcept;


		//-----------------------------------------------------------------------------------------------------
		// トーラス
		// arg1... 半径
		// arg2... リングの太さ
		// arg3... 輪の分割数
		// arg4... 360 度 で円
		// tips... 360度 180度 90度 45度 のみ
		static Mesh* CreateTorus(const float radius, const float thickness, const int tessellation = 20, const int angle = 360) noexcept;
		static Mesh* CreateTorusMV(const float radius, const float thickness, const int tessellation = 20, const int angle = 360) noexcept;

		//-----------------------------------------------------------------------------------------------------
		// 形状タイプから生成
		// arg1... 形状タイプ
		// arg2... 生成用データクラス
		static Mesh* CreateFromTypeDesc(dxe::Mesh::eShapeType shpe_type, dxe::MeshDesc* desc) noexcept;

		//-----------------------------------------------------------------------------------------------------
		// 高さマップから作成
		// arg1... 高さ参照用画像のパス
		// arg2... 幅
		// arg3... 奥行き
		// arg4... 高さの上限値
		// arg5... 横分割数
		// arg6... 縦分割数
		static Mesh* CreateFromHeightMap(const std::string& file_path, const float width, const float depth, const float height_max, const int div_w = 100, const int div_h = 100);
		static Mesh* CreateFromHeightMapMV(const std::string& file_path, const float width, const float depth, const float height_max, const int div_w = 100, const int div_h = 100);
		static Mesh* CreateFromHeightMap(unsigned char* pixels, const int t_width, const int t_height, const int t_bpp, const float width, const float depth, const float height_max, const int div_w = 100, const int div_h = 100);
		static Mesh* CreateFromHeightMapMV(unsigned char* pixels, const int t_width, const int t_height, const int t_bpp, const float width, const float depth, const float height_max, const int div_w = 100, const int div_h = 100);

		//-----------------------------------------------------------------------------------------------------
		// OBJ 形式ファイルからメッシュを作成
		// arg1... フィルパス
		// arg2... スケール( デフォルト1.0f )
		static std::vector<Mesh*> CreateFromFileObj( const std::string& file_path, const float scl = 1.0f );
		static std::vector<Mesh*> CreateFromFileObjMV(const std::string& file_path, const float scl = 1.0f);

		//-----------------------------------------------------------------------------------------------------
		// DXLib が対応している形式ファイルからメッシュを作成
		// arg1... フィルパス
		// arg2... スケール( デフォルト1.0f )
		static Mesh* CreateFromFileMV(const std::string& file_path, const float scl = 1.0f);

		//-----------------------------------------------------------------------------------------------------
		// 独自形式の MVT ファイルから生成
		// arg1... フィルパス
		static std::vector<Mesh*> CreateFromFileMVT(const std::string& file_path);

	private:
		Mesh(){}

		int mv_hdl_ = 0;
		std::shared_ptr<MeshVbo> vbo_			= nullptr;
		std::shared_ptr<MeshDesc> desc_			= nullptr;
		std::string mesh_name_;
		std::vector<std::shared_ptr<Texture>> textures_;

		float						bd_sphere_radius_ = 0;
		tnl::Vector3				bd_box_size_;
		std::vector<uint32_t>		idxs_;
		std::vector<VERTEX3D>		vtxs_;
		float						alpha_ = 1.0f;
		RenderParam					render_param_;
		eMeshFormat					mesh_format_ = eMeshFormat::MESH_FMT_PG;
		eShapeType					shape_type_ = eShapeType::NONE;

		void createPlaneIndex(const int div_w, const int div_h, const bool is_left_cycle);
		void createVBO();

		void drawVtxNormal();
		static Mesh* CreateConvertMV(Mesh* mesh);

	} ;


	constexpr Mesh::fDebugLine operator | (Mesh::fDebugLine left, Mesh::fDebugLine right) {
		return static_cast<Mesh::fDebugLine>(static_cast<int32_t>(left) | static_cast<int32_t>(right));
	}
	constexpr bool operator & (Mesh::fDebugLine left, Mesh::fDebugLine right) {
		return static_cast<bool>(static_cast<int32_t>(left) & static_cast<int32_t>(right));
	}


	//----------------------------------------------------------------------------------------------------------------------
	/*
	*  MVT data format
	* 3      byte : "mvt"
	* 4      byte : sector num
	* 4      byte : all texture num
	* (a) 4  byte : tex name size
	* (b) 4  byte : tex data size
	* a      byte : tex name
	* b      byte : tex data
	* ... loop tex num  ( tex name size -> tex data )
	* 4       byte : sector size
	* (c) 4   byte : mesh name size
	* (d) 4   byte : mesh data size
	* c      byte : mesh name
	* d	     byte : mesh data
	* 4      byte : texture num
	* 4	     byte : tex slot
	* (e) 4  byte : tex name size
	* e      byte : tex name
	* ... loop texture num  ( tex slot -> tex name )
	* 12  byte : position xyz
	* 12  byte : scale xyz
	* 16  byte : rotation xyzw
	* ... loop sector num
	*/

}

