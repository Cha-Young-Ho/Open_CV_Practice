void  draw(Mat  image, Scalar color = Scalar(200, 200, 200))
{
    if (draw_mode == DRAW_RECTANGLE) {
        Rect roi(pt1, pt2);
        fill_rect.push_back(roi);
        rectangle(image, pt1, pt2, color, thickness);
    }
    else if (draw_mode == DRAW_LINE) {
        line(image, pt1, pt2, color, thickness);
    }
    else if (draw_mode == DRAW_BRUSH)
    {
        line(image, pt1, pt2, color, thickness * 3);
        pt1 = pt2;
    }
    else if (draw_mode == ERASE)
    {
        line(image, pt1, pt2, Scalar(255, 255, 255), thickness * 5);
        pt1 = pt2;
    }
    else if (draw_mode == DRAW_CIRCLE)
    {
        Point2d pt3 = pt1 - pt2;
        int  radius = (int)sqrt(pt3.x * pt3.x + pt3.y * pt3.y);   // 두 좌표간 거리
        circle(image, pt1, radius, color, thickness);
    }
    else if (draw_mode == DRAW_ECLIPSE)
    {
        
        Point2d center = (pt1 + pt2) / 2.0;
        Size2d  size = (pt1 - pt2) / 2.0;
        size.width = abs(size.width);
        size.height = abs(size.height);

        ellipse(image, center, size, 0, 0, 360, color, thickness);
    }
    else if (draw_mode == DRAW_POLY) {

        if (flag == 1) {
            line(image, pt1, pt2, color, thickness);
            temp = pt2;
            flag = 2;
        }
        if (flag == 2) {
            line(image, temp, pt2, color, thickness);
            temp = pt2;
        }
        else
            flag = 1;

    }
    else if (draw_mode == DRAW_SPRAY)
    {

        cout << "스프레이 작동" << endl;
        for (int size = 0; size < thickness; size++) {
            Point2d randpt = { rand() % thickness * cos(size) + pt1.x, rand() % thickness * sin(size) + pt1.y };
            circle(image, randpt, 1, color, -1);
        }
        pt1 = pt2;

    }
    else if (draw_mode == DRAW_TEXT)
    {
        string text;
        cout << "텍스트를 입력해주세요. pt1, pt2" << pt1 << pt2 << endl;
        cin >> text;
        putText(image, text, pt2, 2, thickness, color);
        draw_mode = -1;
    }
    else if (draw_mode == FILL_RECTANGLE) {
        for (int i = 0; i < fill_rect.size(); i++)
        {
            if (fill_rect[i].contains(pt2)) {
                rectangle(image, fill_rect[i], color, -1, LINE_4);
            }
        }
    }

    imshow("그림판", image);
}

void  command(int mode)            // 일반 명령 수행
{
    if (mode == PALETTE)                  // 팔레트 영역 클릭
    {
        float  ratio1 = 256.0f / icons[PALETTE].width;      // 너비로 채도 비율 지정
        float  ratio2 = 256.0f / icons[PALETTE].height;      // 높이로 명도 비율 지정

        Point pt = pt2 - icons[PALETTE].tl();                  // 팔레트 내 클릭좌표
        int saturation = cvRound(pt.x * ratio1);
        int value = cvRound((icons[PALETTE].height - pt.y - 1) * ratio2);
        Scalar  hsv(hue, saturation, value);               // HSV 색상 지정

        Mat    m_color = image(icons[COLOR]);         // 색상아이콘 참조
        m_color.setTo(hsv);
        cvtColor(m_color, m_color, COLOR_HSV2BGR);      // HSV → BGR 변환
        rectangle(image, icons[COLOR], Scalar(0, 0, 0), 1);   // 색상 아이콘 테두리색

        Color = Scalar(m_color.at < Vec3b >(10, 10));
    }
    else if (mode == HUE_IDX)                     // 색상인덱스 클릭시
    {
        create_palette(pt2.y, icons[PALETTE]);            // 팔레트 다시 그리기
    }
    else  if (mode == CREAR)
    {
        canvas.setTo(Scalar(255, 255, 255));
        mouse_mode = 0;
        fill_rect.clear();
    }

    else  if (mode == OPEN)
    {
        string name;
        cout << "불러올 파일 이름을 설정해주세요." << endl;
        cin >> name;
        Mat tmp = imread("../image/" + name + ".jpg", 1);
        CV_Assert(!tmp.empty());

        resize(tmp, tmp, canvas.size());
        tmp.copyTo(canvas);
    }
    else  if (mode == SAVE)
    {
        string name;
        cout << "저장할 파일 이름을 설정해주세요." << endl;
        cin >> name;
        imwrite("../image/" + name + ".jpg", canvas);
    }

    else  if (mode == PLUS)
    {
        canvas += Scalar(10, 10, 10);
    }
    else  if (mode == MINUS)
    {

        canvas -= Scalar(10, 10, 10);
    }

    imshow("그림판", image);
}