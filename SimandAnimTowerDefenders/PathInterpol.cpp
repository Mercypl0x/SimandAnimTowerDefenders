#include <SFML/Graphics.hpp>
#include <iostream>

sf::Vector2f CalcCatMullPoint(float t, sf::Vector2f p0, sf::Vector2f p1, sf::Vector2f p2, sf::Vector2f p3)
{
    // t's for matrix
    t = t - (int)t;
    float tt = t * t;
    float ttt = tt * t;

    // Matrix columns with tau from slides
    float col_1 = -ttt + 2.0f * tt - t;
    float col_2 = 3.0f * ttt - 5.0f * tt + 2.0f;
    float col_3 = -3.0f * ttt + 4.0f * tt + t;
    float col_4 = ttt - tt;

    float tx = 0.5f * (p0.x * col_1 + p1.x * col_2 + p2.x * col_3 + p3.x * col_4);
    float ty = 0.5f * (p0.y * col_1 + p1.y * col_2 + p2.y * col_3 + p3.y * col_4);
    sf::Vector2f pointOnSegment(tx, ty);

    return pointOnSegment;
}

struct Enemy {
    std::vector<sf::Vector2f> path; 
    std::vector<float> arcLength;
    float nrOfSplineSamples = 100; 
    float movementSpeed; 
    int currentPathIndex = 1; 
    float distanceTraveled = 0;
    bool spawned;
    sf::CircleShape* shape = new sf::CircleShape();

    void setShapeInitialPos()
    {
        shape->setRadius(10.f);
        shape->setOrigin(shape->getRadius(), shape->getRadius());
        shape->setPosition(path[0]);
    }
    void MoveAlongPath(float dt, float fpsMultiplier)
    {
        float distanceToMove = movementSpeed * dt * fpsMultiplier;
        std::cout << "distance TO Move" << distanceToMove << "\n";
        std::cout << "path size" << path.size() << "\n";
        float totalDistance = distanceTraveled + distanceToMove;
        
        for (size_t i = 0; i < arcLength.size(); i++)
        {
            
            if (arcLength[i] >= totalDistance)
            {
                std::cout << arcLength[i] << "\n";
                std::cout << "totalDistance: " << totalDistance << "\n";
                if (i == 0)
                {
                    if (arcLength[i] == totalDistance)
                    {
                        currentPathIndex = i * nrOfSplineSamples;
                    } 
                    else
                    {
                    
                        currentPathIndex = round(((i) * nrOfSplineSamples + ((i + 1) * nrOfSplineSamples - (i) * nrOfSplineSamples) / (arcLength[i] - 0)) * (totalDistance - 0));
                        std::cout << "currpathidx in calc with 0" << currentPathIndex << "\n";
                    }
                }
                else if (arcLength[i] == totalDistance)
                {
                    currentPathIndex = i * nrOfSplineSamples;
                }
                else 
                {
                    int prevArcLength = i - 1;
                    currentPathIndex = round((i) * nrOfSplineSamples  + (((i + 1) * nrOfSplineSamples - i * nrOfSplineSamples) / (arcLength[i] - arcLength[prevArcLength])) * (totalDistance - arcLength[prevArcLength]));
                    std::cout << "currpathidx in calc with i" << currentPathIndex << "\n";

                }
                break;
            }
            else if (i == arcLength.size() - 1)
            {
                currentPathIndex = path.size() - 1;
            }
        }
       
        std::cout << "curr path idx" << currentPathIndex << "\n";
        shape->setPosition(path[currentPathIndex]);
        distanceTraveled += distanceToMove;
        std::cout << "distance trav'd" << distanceTraveled << "\n";
        if (currentPathIndex == path.size() - 1)
        {
            reachedEnd();
            return;
        }
    }
    void reachedEnd()
    {
        return;
    }

};

void recalcSpline(std::vector<sf::CircleShape*>* splineVector, std::vector<sf::Vector2f> splinePoints)
{
    std::vector<sf::Vector2f> enemyPath;
    for (int segments = 0; segments < splinePoints.size() - 3; segments++)
    {
        int p0 = segments;
        int p1 = segments + 1;
        int p2 = segments + 2;
        int p3 = segments + 3;
        for (float t = 0; t < 1; t += 0.01)
        {
            enemyPath.push_back(CalcCatMullPoint(t, splinePoints[p0],
                                                    splinePoints[p1],
                                                    splinePoints[p2],
                                                    splinePoints[p3]));
        }
    }

    splineVector->clear();
    for (auto point : enemyPath)
    {
        sf::CircleShape* circle = new sf::CircleShape();
        circle->setRadius(2.f);
        circle->setOrigin(circle->getRadius(), circle->getRadius());
        circle->setPosition(point);
        splineVector->push_back(circle);
    }
}
int main()
{
    sf::RenderWindow window(sf::VideoMode(1024, 1024), "SFML works!");

    sf::Clock clock;
    float dt;

    std::vector<sf::CircleShape*> enemies;
    window.setFramerateLimit(10);
    sf::CircleShape enemy(20.f);
    enemy.setPosition(sf::Vector2f(416, 70));
    int tileSize = 32;

    std::vector<sf::CircleShape> returnVector;
    
    
    sf::CircleShape testPoint1(5.f);
    testPoint1.setOrigin(testPoint1.getRadius(), testPoint1.getRadius());
    testPoint1.setPosition(sf::Vector2f(416, 96));

    
    sf::CircleShape testPoint0(5.f);
    testPoint0.setOrigin(testPoint0.getRadius(), testPoint0.getRadius());
    testPoint0.setPosition(testPoint1.getPosition().x, testPoint1.getPosition().y + -3 * tileSize);
    enemies.push_back(&testPoint0);

    enemies.push_back(&testPoint1);

    sf::CircleShape testPoint2(5.f);
    testPoint2.setOrigin(testPoint2.getRadius(), testPoint2.getRadius());
    testPoint2.setPosition(testPoint1.getPosition().x + tileSize / 2, testPoint1.getPosition().y + 3 * tileSize);
    enemies.push_back(&testPoint2);

    sf::CircleShape testPoint3(5.f);
    testPoint3.setOrigin(testPoint3.getRadius(), testPoint3.getRadius());
    testPoint3.setPosition(testPoint2.getPosition().x + tileSize / 2, testPoint2.getPosition().y + 12 * tileSize);
    enemies.push_back(&testPoint3);

    sf::CircleShape testPoint4(5.f);
    testPoint4.setOrigin(testPoint4.getRadius(), testPoint4.getRadius());
    testPoint4.setPosition(testPoint3.getPosition().x + 6 * tileSize, testPoint3.getPosition().y);
    enemies.push_back(&testPoint4);

    sf::CircleShape testPoint5(5.f);
    testPoint5.setOrigin(testPoint5.getRadius(), testPoint5.getRadius());
    testPoint5.setPosition(testPoint4.getPosition().x + 2 * tileSize, testPoint4.getPosition().y + 3 * tileSize);
    enemies.push_back(&testPoint5);

    sf::CircleShape testPoint6(5.f);
    testPoint6.setOrigin(testPoint6.getRadius(), testPoint6.getRadius());
    testPoint6.setPosition(testPoint5.getPosition().x + 1 * tileSize, testPoint5.getPosition().y + 3 * tileSize);
    enemies.push_back(&testPoint6);

    sf::CircleShape testPoint7(5.f);
    testPoint7.setOrigin(testPoint7.getRadius(), testPoint7.getRadius());
    testPoint7.setPosition(testPoint6.getPosition().x + -1 * tileSize, testPoint6.getPosition().y + 3 * tileSize);
    enemies.push_back(&testPoint7);

    sf::CircleShape testPoint8(5.f);
    testPoint8.setOrigin(testPoint8.getRadius(), testPoint8.getRadius());
    testPoint8.setPosition(testPoint7.getPosition().x + -4 * tileSize, testPoint7.getPosition().y + 3 * tileSize);
    enemies.push_back(&testPoint8);

    sf::CircleShape testPoint9(5.f);
    testPoint9.setOrigin(testPoint9.getRadius(), testPoint9.getRadius());
    testPoint9.setPosition(testPoint8.getPosition().x + -14 * tileSize, testPoint8.getPosition().y + 1 * tileSize);
    enemies.push_back(&testPoint9);

    sf::CircleShape testPoint10(5.f);
    testPoint10.setOrigin(testPoint10.getRadius(), testPoint10.getRadius());
    testPoint10.setPosition(testPoint9.getPosition().x, testPoint9.getPosition().y + -5 * tileSize);
    enemies.push_back(&testPoint10);

    sf::CircleShape testPoint11(5.f);
    testPoint11.setOrigin(testPoint11.getRadius(), testPoint11.getRadius());
    testPoint11.setPosition(testPoint10.getPosition().x - 3 * tileSize, testPoint10.getPosition().y + -1 * tileSize);
    enemies.push_back(&testPoint11);


    std::vector<sf::Vector2f> enemyPathRough;
    for (auto point : enemies)
    {
        enemyPathRough.push_back(point->getPosition());
    }
    // enemies.push_back(&enemy);

    sf::Texture mapTexture; 
    std::cout << mapTexture.loadFromFile("../textures/map1.png");
    sf::Sprite mapSprite(mapTexture);
    sf::Vector2u textureSize = mapTexture.getSize();
    sf::Vector2f maxViewSize(1024.f, 1024.f);
    sf::Vector2f minViewSize(200.f, 200.f);

    

    sf::View view(sf::FloatRect(0, 0, 1024 , 1024));

    mapSprite.scale(2,2);
    mapSprite.setOrigin(textureSize.x/2, textureSize.y/2);
    mapSprite.setPosition(512, 512);
    
    enemy.setOrigin(enemy.getRadius(), enemy.getRadius());
    

  
    enemy.setFillColor(sf::Color::Green);
    

    std::vector<sf::Vector2f> enemyPath; 
    for (int segments = 0; segments < enemyPathRough.size() - 3; segments++)
    {
        int p0 = segments;
        int p1 = segments + 1;
        int p2 = segments + 2;
        int p3 = segments + 3;
        for (float t = 0; t < 1; t += 0.01)
        {
            enemyPath.push_back(CalcCatMullPoint(t, enemyPathRough[p0],
                                                    enemyPathRough[p1],
                                                    enemyPathRough[p2],
                                                    enemyPathRough[p3]));
        }
    }
    std::vector<sf::CircleShape*> spline; 


    for (auto point : enemyPath)
    {
        sf::CircleShape* circle = new sf::CircleShape();
        circle->setRadius(2.f);
        circle->setOrigin(circle->getRadius(), circle->getRadius());
        circle->setPosition(point);
        spline.push_back(circle);
    }

    Enemy* enemy1 = new Enemy();
    enemy1->movementSpeed = 10.f;
    enemy1->path = enemyPath;
    enemy1->shape->setOrigin(enemy1->shape->getRadius(), enemy1->shape->getRadius());
    enemy1->setShapeInitialPos();
    
    std::vector<float> segmentChordLengths;
   
    std::vector<std::pair<sf::Vector2f, sf::Vector2f>> chordLengthlines; 
    float chordLength = 0.f; 
    for (int first = 1; first < enemyPathRough.size() - 2; first++)
    {
        int second = first + 1;
        int segmentChordLenghtsIterator = first - 2; 
        sf::Vector2f distanceVec = enemyPathRough[second] - enemyPathRough[first];
        chordLengthlines.push_back(std::make_pair(enemyPathRough[first], enemyPathRough[second]));
        float distance = std::sqrt(distanceVec.x * distanceVec.x + distanceVec.y * distanceVec.y);
        chordLength += distance;
        if (first != 1)
            segmentChordLengths.push_back(distance + segmentChordLengths[segmentChordLenghtsIterator]);
        else
            segmentChordLengths.push_back(distance);
    }
    
    enemy1->arcLength = segmentChordLengths;
    std::cout << "chordLength: " << chordLength << "\n";
    while (window.isOpen())
    {

        sf::Event event;
        while (window.pollEvent(event))
        {
            switch (event.type)
            {
                case sf::Event::Closed:
                {
                    window.close();

                }

                case sf::Event::MouseWheelMoved:
                {
                    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                    sf::Vector2f translatedMousePos = window.mapPixelToCoords(mousePos);
                    view.setCenter(static_cast<sf::Vector2f>(translatedMousePos));

                    if (event.mouseWheel.delta <= 0)
                    {

                        if (view.getSize().x < maxViewSize.x)
                        { 
                            view.zoom(1.1);
                        }
                        else
                        {
                            view.reset(sf::FloatRect(0, 0, 1024, 1024));
                        }

                    }
                    else if (event.mouseWheel.delta > 0)
                    {
                        if (view.getSize().x > minViewSize.y)
                        {
                            view.zoom(0.9);
                        }
                    }
                }
                case sf::Event::MouseButtonPressed:
                    if (event.mouseButton.button == sf::Mouse::Left)
                    {

                    }
                  
                case sf::Event::MouseButtonReleased:
                    if (event.mouseButton.button == sf::Mouse::Left)
                    {

                    }
                case sf::Event::KeyPressed: 
                {
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
                    {
                        enemy1->MoveAlongPath(dt, 60.f);
                    } 
                    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
                    {
                        enemy1->spawned = true;
                      
                    }

                }
              
            }
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
            {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                sf::Vector2f translatedMousePos = window.mapPixelToCoords(mousePos);
                sf::CircleShape* selectedShape = NULL;
                for (auto point : enemies)
                {
                    if ((translatedMousePos.x <= point->getPosition().x + point->getRadius() &&
                        translatedMousePos.x >= point->getPosition().x - point->getRadius()) &&
                        (translatedMousePos.y <= point->getPosition().y + point->getRadius() &&
                            translatedMousePos.y >= point->getPosition().y - point->getRadius()))
                    {
                        selectedShape = point;
                    }
                }
                if (selectedShape != NULL)
                { 
                    selectedShape->setPosition(translatedMousePos);
                    std::vector<sf::Vector2f> newEnemyPath;
                    for (auto point : enemies)
                    {
                        newEnemyPath.push_back(point->getPosition());
                    }
                    recalcSpline(&spline, newEnemyPath);
                }

                

            }
        }

        dt = clock.restart().asSeconds();
        if (enemy1->spawned)
            enemy1->MoveAlongPath(dt, 10.f);

        window.setView(view);
        window.clear();
        window.draw(mapSprite);
        window.draw(*(enemy1->shape));
        for (sf::CircleShape* enemy : enemies)
        {
            window.draw(*enemy);
        }
        for (auto line : chordLengthlines)
        {
            sf::Vertex vertices[2] =
            {
                sf::Vertex(line.first, sf::Color::Red),
                sf::Vertex(line.second, sf::Color::Red)
            };
            window.draw(vertices, 2, sf::Lines);
        }
        for (auto point : spline)
        {
            window.draw(*point);
        }
        window.display();
    }

    return 0;
}