//
// Created by 15411 on 2025/3/2.
//
void Widget::on_pushButton_16_clicked() // 保存地图
{
    // 固定保存路径
    QString dirPath = "D:/MyMaps/";
    QDir dir(dirPath);
    if (!dir.exists()) {
        dir.mkpath(dirPath); // 如果目录不存在，则创建
    }

    QString fileName = dirPath + "map1.whmap"; // 固定文件名

    QFile file(fileName);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "错误", "保存失败: " + file.errorString());
        return;
    }

    QTextStream out(&file);
    // 保存背景图片路径
    out << "[Pic]\n"
        << pic << "\n\n";

    // 保存点信息
    out << "[Points]\n";
    for (int i = 1; i <= p_num; i++) { // 只保存有效点
        const auto &p = point[i];
        out << p.location.x() << ","
            << p.location.y() << ","
            << p.label.replace(",","_") << "," // 转义逗号
            << p.time << "\n";
    }

    // 保存边信息
    out << "\n[Lines]\n";
    for (int i = 1; i <= l_num; i++) { // 只保存有效边
        const auto &l = line[i];
        out << l.st.replace(",","_") << ","
            << l.ed.replace(",","_") << ","
            << l.len << "\n";
    }

    file.close();
    QMessageBox::information(this, "成功", QString("地图已保存至:\n%1").arg(fileName));
}

void Widget::on_pushButton_17_clicked() // 载入地图
{
    // 固定载入路径
    QString fileName = "D:/MyMaps/map1.whmap";

    // 检查文件是否存在
    if (!QFile::exists(fileName)) {
        QMessageBox::warning(this, "错误", "地图文件不存在: " + fileName);
        return;
    }

    // 重置状态
    p_num = l_num = 0;
    point.clear();
    line.clear();
    point.push_back(points());
    line.push_back(lines());

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "错误", "读取失败: " + file.errorString());
        return;
    }

    QTextStream in(&file);
    QString cur;

    while (!in.atEnd()) {
        QString lineStr = in.readLine().trimmed();
        if (lineStr.startsWith("[Pic]")) {
            cur = "Pic";
            continue;
        } else if (lineStr.startsWith("[Points]")) {
            cur = "Points";
            continue;
        } else if (lineStr.startsWith("[Lines]")) {
            cur = "Lines";
            continue;
        }

        if (cur == "Pic") {
            pic = lineStr; // 恢复背景图片路径
            ui->label_6->setPixmap(QPixmap(pic)); // 更新背景显示
        }
        else if (cur == "Points") {
            QStringList parts = lineStr.split(",");
            if (parts.size() != 4) continue;

            points p;
            p.location.setX(parts[0].toInt());
            p.location.setY(parts[1].toInt());
            p.label = parts[2].replace("_",","); // 还原转义符
            p.time = parts[3].toInt();

            point.push_back(p);
            p_num++;
        }
        else if (cur == "Lines") {
            QStringList parts = lineStr.split(",");
            if (parts.size() != 3) continue;

            lines l;
            l.st = parts[0].replace("_",",");
            l.ed = parts[1].replace("_",",");
            l.len = parts[2].toInt();

            line.push_back(l);
            l_num++;
        }
    }

    file.close();
    update(); // 重绘界面
    QMessageBox::information(this, "成功", QString("已载入地图:\n%1").arg(fileName));
}