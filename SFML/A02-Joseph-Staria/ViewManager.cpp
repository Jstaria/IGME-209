#include "ViewManager.h"

ViewManager* ViewManager::s_instance = nullptr;

ViewManager::ViewManager(RenderWindow& window, TankMap* tankMap_ptr, vector<Tank*> tank_ptr)
{
    s_instance = this;

    m_free_view = window.getDefaultView();
    m_free_view.setSize(tankMap_ptr->GetMapSizePixel());
    m_free_view.setCenter(tankMap_ptr->GetMapSizePixel() / 2.0f);

    m_focus_view = window.getDefaultView();
    m_focus_view.setSize(window.getDefaultView().getSize() * 1.5f);
    m_curret_focus = 0;

    window.setView(m_free_view);
    m_view_type = VIEW_TYPE::FREE;

    m_tank_arr = tank_ptr;

    m_tank_left = tank_ptr.size();

    font.loadFromFile("Assets/Fonts/arial.ttf");
}

void ViewManager::Update(RenderWindow& window, float deltaSeconds)
{
    if (m_is_full_camera_button_down == false && Keyboard::isKeyPressed(Keyboard::C)) {
        if (m_view_type == VIEW_TYPE::FOCUS) {
            m_view_type = VIEW_TYPE::FREE;
            
        }
        else if (m_view_type == VIEW_TYPE::FREE) {
            m_view_type = VIEW_TYPE::FOCUS;
            
            m_focus_view.setCenter(m_tank_arr[m_curret_focus]->GetPosition());
        }
    }

    if (m_view_type == VIEW_TYPE::FOCUS) {
        if (m_is_next_button_down == false && Keyboard::isKeyPressed(Keyboard::Space)) {
            m_curret_focus = (m_curret_focus + 1) % m_tank_arr.size();
        }

        m_focus_view.setCenter(m_tank_arr[m_curret_focus]->GetPosition());
        
        if (Keyboard::isKeyPressed(Keyboard::I) || Keyboard::isKeyPressed(Keyboard::Add)) {
            m_focus_view.zoom(1.0f - 1.0f * deltaSeconds);
        }

        if (Keyboard::isKeyPressed(Keyboard::O) || Keyboard::isKeyPressed(Keyboard::Subtract)) {
            m_focus_view.zoom(1.0f + 1.0f * deltaSeconds);
        }
        window.setView(m_focus_view);
    }
    else {
        if (Keyboard::isKeyPressed(Keyboard::Left)) {
            Vector2f size = m_free_view.getSize();
            m_free_view.move(-size.x / 2.0f * deltaSeconds, 0.0f);
        }
        if (Keyboard::isKeyPressed(Keyboard::Right)) {
            Vector2f size = m_free_view.getSize();
            m_free_view.move(size.x / 2.0f * deltaSeconds, 0.0f);
        }
        if (Keyboard::isKeyPressed(Keyboard::Up)) {
            Vector2f size = m_free_view.getSize();
            m_free_view.move(0.0f, -size.y / 2.0f * deltaSeconds);
        }
        if (Keyboard::isKeyPressed(Keyboard::Down)) {
            Vector2f size = m_free_view.getSize();
            m_free_view.move(0.0f, size.y / 2.0f * deltaSeconds);
        }

        if (Keyboard::isKeyPressed(Keyboard::I) || Keyboard::isKeyPressed(Keyboard::Add)) {
            m_free_view.zoom(1.0f - 1.0f * deltaSeconds);
        }

        if (Keyboard::isKeyPressed(Keyboard::O) || Keyboard::isKeyPressed(Keyboard::Subtract)) {
            m_free_view.zoom(1.0f + 1.0f * deltaSeconds);
        }
        window.setView(m_free_view);
    }

    m_is_full_camera_button_down = Keyboard::isKeyPressed(Keyboard::C);
    m_is_next_button_down = Keyboard::isKeyPressed(Keyboard::Space);
}

void ViewManager::Render(RenderWindow& window)
{
    View game_view = window.getView();
    View ui_view = window.getDefaultView();
    window.setView(ui_view);

    if (m_view_type == VIEW_TYPE::FOCUS) {
        Text text;
        text.setFont(font);
        Tank* tank = m_tank_arr[m_curret_focus];
        string name = tank->GetName();
        string hp = "HP: " + to_string((int)tank->GetHealth()) + "/" + to_string((int)tank->GetMaxHealth());
        string engine = "Engine: " + to_string(tank->GetEngineUpgrade());
        string armor = "Armor: " + to_string(tank->GetArmorUpgrade());
        string turret = "Turret: " + to_string(tank->GetTurretUpgrade());

        text.setString(
            name + "\n"
            + hp + "\n"
            + engine + "\n"
            + armor + "\n"
            + turret
        );


        text.setCharacterSize(32);
        text.setPosition(10, 0);

        window.draw(text);
    }

    {
        Text text;
        text.setFont(font);

        text.setString(to_string(m_tank_left) + "/" + to_string(m_tank_arr.size()));

        text.setCharacterSize(48);
        text.setPosition(400, 0);

        window.draw(text);
    }
    {
        RectangleShape  shape = RectangleShape();
        shape.setFillColor(Color(100, 200, 100, 50));
        shape.setSize(Vector2f(window.getSize().x / 4, window.getSize().x / 8));
        // shape.setOrigin(0.0f ,shape.getSize() / 2.0f);
        shape.setPosition(0, window.getSize().x * 7.0f / 8.0f);

        window.draw(shape);

        Text kill_text;
        kill_text.setFont(font);
        string kill_str = "";

        int count = 0;
        for (int i = kill_message.size() - 1; i >= 0; i--) {
            kill_str += kill_message[i];
            count++;
            if (count >= 5)
                break;
        }

        
        kill_text.setString(kill_str);
        kill_text.setCharacterSize(16);
        kill_text.setPosition(0, 850);

        window.draw(kill_text);
    }

    if (m_tank_left == 0) {
        RectangleShape  shape = RectangleShape();
        shape.setFillColor(Color(100, 100, 200, 100));
        shape.setSize(Vector2f(window.getSize().x / 1.5, window.getSize().x / 1.2));
        shape.setOrigin(shape.getSize() / 2.0f);
        shape.setPosition(Vector2f(window.getSize().x / 2, window.getSize().x / 2));

        Text text;
        text.setFont(font);

        string str = "";

        // Last Surviver
        str += "Last Survivor:\n    " + last_surviver + "\n";
        // Last Surviver Runner-Up
        str += "Last Survivor Runner-Up:\n";
        for (int i = 0; i < last_surviver_runner_up.size(); i++) {
            str += "    " + last_surviver_runner_up[i] + "\n";
        }
        // First Blood
        str += "First Blood:\n    " + first_blood + "\n";
        // Master of Destruction

        str += "Master of Destruction: " + to_string(master_of_destruction_num) + "\n";
        for (int i = 0; i < master_of_destruction.size(); i++) {
            str += "    " + master_of_destruction[i] + "\n";
        }
        // Tank Among Tanks
        str += "Tank Among Tanks: " + to_string(tank_among_tanks_num) + "\n";
        for (int i = 0; i < tank_among_tanks.size(); i++) {
            str += "    " + tank_among_tanks[i] + "\n";
        }
        // Best Engineer
        str += "Best Engineer:\n";
        for (int i = 0; i < best_engineer.size(); i++) {
            str += "    " + best_engineer[i] + "\n";
        }

        text.setString(str);

        text.setCharacterSize(32);
        text.setPosition(200, 200);

        window.draw(shape);
        window.draw(text);
    }

    window.setView(game_view);
}

View ViewManager::GetFullView()
{
    return m_free_view;
}

void ViewManager::AddKill(string killer, string target)
{
    if (killer._Equal("fog") == true) {
        kill_message.push_back(target + " destroyed by " + "poison fog\n");
    }
    else{
        kill_message.push_back(killer + " destroys " + target + "\n");
        if (is_first_blood == false) {
            is_first_blood = true;
            first_blood = killer;
        }
    }

    m_tank_left--;

    if (m_tank_left == 0) {
        last_surviver = target;

        {
            int max_kills = 0;
            int max_upgrade = 0;
            int max_tank = 0;

            for (int i = 0; i < m_tank_arr.size(); i++) {
                Tank* tank = m_tank_arr[i];

                if (tank->kills > max_kills) {
                    max_kills = m_tank_arr[i]->kills;
                }

                int upgrade = tank->GetEngineUpgrade() + tank->GetArmorUpgrade() + tank->GetTurretUpgrade();
                if (upgrade > max_upgrade) {
                    max_upgrade = upgrade;
                }
                
                if (tank->total_hit_get > max_tank)
                    max_tank = tank->total_hit_get;
            }

            master_of_destruction_num = max_kills;
            tank_among_tanks_num = max_tank;

            for (int i = 0; i < m_tank_arr.size(); i++) {
                Tank* tank = m_tank_arr[i];

                if (tank->kills == max_kills) {
                    master_of_destruction.push_back(m_tank_arr[i]->GetName());
                }

                int upgrade = tank->GetEngineUpgrade() + tank->GetArmorUpgrade() + tank->GetTurretUpgrade();
                if (upgrade == max_upgrade) {
                    best_engineer.push_back(m_tank_arr[i]->GetName());
                }

                if (tank->total_hit_get == max_tank)
                    tank_among_tanks.push_back(m_tank_arr[i]->GetName());
            }
        }

        {
            int max_upgrade = 0;
        }

    }
    else if (m_tank_left < 5) {
        last_surviver_runner_up.push_back(target);
    }
}
