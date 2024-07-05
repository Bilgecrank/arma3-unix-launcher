#pragma once

#include "ui_mainwindow.h"

#include <thread>

#include <QCheckBox>
#include <QLineEdit>
#include <QMainWindow>
#include <QMenu>
#include <QTableWidgetItem>
#include <QTimer>

#include <nlohmann/json.hpp>

#include <arma3client.hpp>
#include <steam_integration_stub.hpp>

#include "settings.hpp"
#include "ui_mod.hpp"

class MainWindow : public QMainWindow
{
        Q_OBJECT

    public:
        explicit MainWindow(std::unique_ptr<ARMA3::Client> arma3_client, std::filesystem::path const &config_file_path,
                            bool use_steam_integration, QWidget *parent = nullptr);
        ~MainWindow();

    private slots:
        void on_button_start_clicked();
        void on_button_add_custom_mod_clicked();
        void on_button_remove_custom_mod_clicked();
        void on_button_parameter_file_open_clicked();
        void on_button_mod_preset_open_clicked();
        void on_button_mod_preset_save_clicked();
        void on_button_quit_clicked();
        void on_button_export_workshop_to_txt_clicked();

        void on_checkbox_extra_threads_stateChanged(int arg1);
        void on_checkbox_cpu_count_stateChanged(int arg1);
        void on_checkbox_parameter_file_stateChanged(int arg1);
        void on_checkbox_profiles_stateChanged(int arg1);
        void on_checkbox_world_stateChanged(int arg1);
        void on_checkbox_custom_parameters_stateChanged(int arg1);
        void on_checkbox_server_address_stateChanged(int arg1);
        void on_checkbox_server_port_stateChanged(int arg1);
        void on_checkbox_server_password_stateChanged(int arg1);
        void on_checkbox_profile_stateChanged(int arg1);
        void on_checkbox_environment_variables_stateChanged(int arg1);

        void check_if_arma_is_running();
        void check_steam_api();

        void on_combobox_theme_currentTextChanged(QString const &theme);

        void on_action_mods_save_json_triggered();
        void on_action_mods_save_html_triggered();

        void on_updateNotification(bool is_there_a_new_version, std::string content);

    private:
        Ui::MainWindow *ui;
        QMenu menu_button_save;
        QTimer arma_status_checker;
        QTimer steam_api_checker;

        std::unique_ptr<ARMA3::Client> client;
        std::unique_ptr<Steam::IntegrationStub> steam_integration;

        std::filesystem::path config_file;
        std::thread update_notification_thread;
        Settings manager;

        std::vector<std::uint64_t> mods_to_enable;

        void load_mods_from_json(nlohmann::json preset);
        void load_mods_from_html(std::string const &path);
        void propose_subscribing_to_mods(std::string const &mod_list_message, std::vector<std::uint64_t> const &workshop_mods);

        void put_mods_from_ui_to_manager_settings();

        void update_mod_selection_counters(int workshop_mod_count, int custom_mod_count);

        void setup_steam_integration();
        void on_workshop_mod_installed(Steam::Structs::ItemDownloadedInfo const &info);

        void on_mods_disable_all_mods();
        void on_mods_enable_all_mods();

        void initialize_theme_combobox();

        std::string ui_path_to_full_path(std::string ui_path) const;
        std::string full_path_to_ui_path(std::string ui_path) const;
        bool is_workshop_mod(std::string const &path_or_workshop_id);
        Mod get_mod(std::string const &path_or_workshop_id);
};
