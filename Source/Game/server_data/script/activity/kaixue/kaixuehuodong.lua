--��ѧ�

--[[Yuan_Dan_NPC = {}
Yuan_Dan_NPC[1] = {mapcrc=3017298127, monsterid=1531201, x=2209, y=6646, z=2223, x2=2156, y2=6646, z2=2223}
Yuan_Dan_NPC[2] = {mapcrc=3017298383, monsterid=1531202, x=913, y=10000, z=1943, x2=863, y2=10000, z2=1963}
Yuan_Dan_NPC[3] = {mapcrc=3017299663, monsterid=1531203, x=2071, y=18338, z=2405, x2=2134, y2=18338, z2=2423}
Yuan_Dan_NPC[4] = {mapcrc=3017299919, monsterid=1531204, x=1027, y=20140, z=977, x2=976, y2=20140, z2=954}
Yuan_Dan_NPC[5] = {mapcrc=3017299151, monsterid=1531205, x=2064, y=6365, z=1794, x2=2032, y2=6365, z2=1730}
Yuan_Dan_NPC[6] = {mapcrc=3017299407, monsterid=1531205, x=1865, y=14024, z=2290, x2=1744, y2=14024, z2=2309}

Yuan_Dan_Exp = {}
Yuan_Dan_Exp[0] = 380
Yuan_Dan_Exp[1] = 780
Yuan_Dan_Exp[2] = 1280
Yuan_Dan_Exp[3] = 1890
Yuan_Dan_Exp[4] = 2550
Yuan_Dan_Exp[5] = 3550
Yuan_Dan_Exp[6] = 4680
Yuan_Dan_Exp[7] = 6380
Yuan_Dan_Exp[8] = 8160
Yuan_Dan_Exp[9] = 10340
Yuan_Dan_Exp[10] = 11160

YuanDan_TimerMin = 0

m02_yuandan_point={}
m02_yuandan_point[1]={mapcrc=3017298127, x=2296, y=5756, z=2204}
m02_yuandan_point[2]={mapcrc=3017298127, x=3265, y=4885, z=1597}
m02_yuandan_point[3]={mapcrc=3017298127, x=3184, y=8402, z=3356}
m02_yuandan_point[4]={mapcrc=3017298127, x=1925, y=8000, z=3624}

m03_yuandan_point={}
m03_yuandan_point[1]={mapcrc=3017298383, x=967, y=10000, z=2079}
m03_yuandan_point[2]={mapcrc=3017298383, x=537, y=8798, z=1223}
m03_yuandan_point[3]={mapcrc=3017298383, x=2231, y=10964, z=2495}
m03_yuandan_point[4]={mapcrc=3017298383, x=2520, y=13100, z=1616}

m04_yuandan_point={}
m04_yuandan_point[1]={mapcrc=3017299663, x=2074, y=18342, z=2425}
m04_yuandan_point[2]={mapcrc=3017299663, x=1099, y=13199, z=1683}
m04_yuandan_point[3]={mapcrc=3017299663, x=3053, y=23613, z=1462}
m04_yuandan_point[4]={mapcrc=3017299663, x=2745, y=13900, z=800}

m05_yuandan_point={}
m05_yuandan_point[1]={mapcrc=3017299919, x=1190, y=20140, z=873}
m05_yuandan_point[2]={mapcrc=3017299919, x=1475, y=16700, z=1748}
m05_yuandan_point[3]={mapcrc=3017299919, x=2426, y=16352, z=2530}
m05_yuandan_point[4]={mapcrc=3017299919, x=2361, y=17572, z=865}

m06_yuandan_point={}
m06_yuandan_point[1]={mapcrc=3017299151, x=2089, y=6365, z=1791}
m06_yuandan_point[2]={mapcrc=3017299151, x=1044, y=3785, z=1402}
m06_yuandan_point[3]={mapcrc=3017299151, x=1145, y=7900, z=2098}
--m06_yuandan_point[4]={mapcrc=3017299151, x=1460, y=8801, z=500}

m07_yuandan_point={}
m07_yuandan_point[1]={mapcrc=3017299407, x=1892, y=14024, z=2476}
m07_yuandan_point[2]={mapcrc=3017299407, x=2294, y=9860, z=1386}
m07_yuandan_point[3]={mapcrc=3017299407, x=969, y=8226, z=2309}
m07_yuandan_point[4]={mapcrc=3017299407, x=1138, y=10497, z=1041}

allmap_yuandan_point={}
allmap_yuandan_point[1]={maptable=m02_yuandan_point, pointnum=4}
allmap_yuandan_point[2]={maptable=m03_yuandan_point, pointnum=4}
allmap_yuandan_point[3]={maptable=m04_yuandan_point, pointnum=4}
allmap_yuandan_point[4]={maptable=m05_yuandan_point, pointnum=4}
allmap_yuandan_point[5]={maptable=m06_yuandan_point, pointnum=3}
allmap_yuandan_point[6]={maptable=m07_yuandan_point, pointnum=4}]]


xiaoerlang = 0
huanyaolaoshi = 0

function KaiXue_OnStart(actID)
	local k = act.GetActScriptData(77, 1, 10)
	local curyear = tonumber(os.date("%Y"))
	if k == 0 or k == nil or k ~= curyear then
	    for i=0, 9 do
		    act.SetActScriptData(77, 1, i, 0)
		end
		act.SetActScriptData(77, 1, 10, curyear)
		act.SaveActScriptData(77)
	end

	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 100, 2350)  --��ѧ����������ף���е�ѧ����Ҷ������µ�һ����ѧҵ�����������³ɣ���ҸϿ�ȥ���������ҽ�ѧ�𷢷����˽������ȡ��ѧ��ɣ�
	msg.DispatchWorldMsgEvent(MsgID)

	xiaoerlang = map.MapCreateCreature(3017298127, -1, 4900439, 2158, 6646, 2245)
	huanyaolaoshi = map.MapCreateCreature(3017298127, -1, 4900440, 2158, 6646, 2253)
	map.MapCreateCreature(3017298127, -1, 4900441, 2163, 6646, 2253)

end

function KaiXue_OnTimerMin(actID)

	local curhour = os.date("%H")
	local curmin = os.date("%M")

	local time5 = {}
	time5[1]="00"
	time5[2]="05"
	time5[3]="10"
	time5[4]="15"
	time5[5]="20"
	time5[6]="25"
	time5[7]="30"
	time5[8]="35"
	time5[9]="40"
	time5[10]="45"
	time5[11]="50"
	time5[12]="55"

	local TimeIs5=0

	for i=1,12 do
		if curmin==time5[i] then
			TimeIs5=1
		end
	end


	if (curhour=="19" or curhour=="20" or curhour=="21" or curhour=="22") and TimeIs5==1 then
	    for i=1,6 do
			local temppoint=math.random(allmap_yuandan_point[i].pointnum)
			local mapcrc=allmap_yuandan_point[i].maptable[temppoint].mapcrc
			local x=allmap_yuandan_point[i].maptable[temppoint].x
			local y=allmap_yuandan_point[i].maptable[temppoint].y
			local z=allmap_yuandan_point[i].maptable[temppoint].z

			map.MapCreateColCreature(mapcrc, -1, 1534124+i, x, y, z,1)
		end
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 13, 1)
		msg.AddMsgEvent(MsgID, 1, 2351)  --��ѧ΢���������˸��������У�������ץס���Ի�÷��Ľ�����
		msg.DispatchWorldMsgEvent(MsgID)
	end

	for randindex=1,6 do

		local mapindex = Yuan_Dan_NPC[randindex].mapcrc
		local monster = Yuan_Dan_NPC[randindex].monsterid + 2918
		if mapindex == 3017299407 then
			monster = 1534124
		end
		local k = math.random(500,520)

		map.MapCreateCreature(mapindex, -1, monster, map_list[mapindex].map[k].x, map_list[mapindex].map[k].y, map_list[mapindex].map[k].z)
    end

	if curmin == "30" or curmin == "00" then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 13, 1)
		msg.AddMsgEvent(MsgID, 1, 2352)  --��ѧ��ڼ�ÿ���Ӷ���ˢ����ѧС����ץ�������˿��Ի�ò��ٺö���Ŷ��
        msg.DispatchWorldMsgEvent(MsgID)
	end
end

-- npc����ʱ�䵽����������ڸ�������ɾ����npc
function KaiXue_OnEnd(actID)
	map.MapDeleteCreature(3017298127, -1, xiaoerlang)
	map.MapDeleteCreature(3017298127, -1, huanyaolaoshi)
end

--ע��û������¼�
aux.RegisterActEvent(77, 2, "KaiXue_OnStart")
aux.RegisterActEvent(77, 3, "KaiXue_OnEnd")
aux.RegisterActEvent(77, 4, "KaiXue_OnTimerMin")

--�������ʱ
function q20188_OnComplete(MapID, InstanceID, questID, ownerID, enderID)
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 13, 1)
	msg.AddMsgEvent(MsgID, 1, 2353)  --<�����>��800���׷�ñ�ڽ�ѧ�𷢷��˴���ȡ�˶��Ƚ�ѧ��Ľ�����
    msg.AddMsgEvent(MsgID, 2, ownerID)
	msg.DispatchWorldMsgEvent(MsgID)
end

--ע��
aux.RegisterQuestEvent(20188, 1, "q20188_OnComplete")

--�������ʱ
function q20189_OnComplete(MapID, InstanceID, questID, ownerID, enderID)
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 13, 1)
	msg.AddMsgEvent(MsgID, 1, 2354)  --<�����>��360���׷�ñ�ڽ�ѧ�𷢷��˴���ȡ�����Ƚ�ѧ��Ľ�����
    msg.AddMsgEvent(MsgID, 2, ownerID)
	msg.DispatchWorldMsgEvent(MsgID)
end

--ע��
aux.RegisterQuestEvent(20189, 1, "q20189_OnComplete")

--�������ʦ4900440�Ի�
function n4900440_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
    if TalkIndex == -1 then
        local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20, 2355) -- "������ʦ��\n    ��������ҵ�����Ұɣ�Ҫ�������ҵ��á��š��Ļ����Ϳ��Դ���������ȡһ���׷�ñŶ��"
		msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
		msg.AddMsgEvent(MsgID, 21, 5) -- ȷ����ť
		msg.AddMsgEvent(MsgID, 1, 2356) -- ���׷�ñ�������а�ǰ��������
		msg.AddMsgEvent(MsgID, 21, 4)   --ȡ����ť
		msg.AddMsgEvent(MsgID, 1, 2357)   --"�ύ��ɵ���ҵ"
		msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
    elseif TalkIndex == 4 then
        local a = role.GetRoleItemNum(RoleID, 3303210)
		local b = role.GetRoleItemNum(RoleID, 3303211)
		local c = role.GetRoleItemNum(RoleID, 3303212)
		local d = role.GetRoleItemNum(RoleID, 3303213)
		if a > 0 then
		    local FreeSize = role.GetBagFreeSize(RoleID)
			if FreeSize > 0 then
				role.RemoveFromRole(MapID, InstanceID, RoleID, 3303210, 1, 420)
				role.AddRoleItem(MapID, InstanceID, RoleID, 3303206, 1, 1, 8, 420)
				HuoDeLeiFengMao(RoleID)
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 2358) -- "������ʦ��\n    ��ε���ҵ���úܺ�Ŷ�����Ե��ţ�����һ���׷�ñ��Ϊ������"
				msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
				msg.AddMsgEvent(MsgID, 21, -1) -- ȷ����ť
				msg.AddMsgEvent(MsgID, 1, 21) -- ��ȷ����
				msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
			else
			    local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 2359) -- "������ʦ��\n    ��ı��������ˣ�ȥ����һ�������ɣ�"
				msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
				msg.AddMsgEvent(MsgID, 21, -1) -- ȷ����ť
				msg.AddMsgEvent(MsgID, 1, 21) -- ��ȷ����
				msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
			end
		else
			if b > 0 then
			    role.RemoveFromRole(MapID, InstanceID, RoleID, 3303211, 1, 420)
				local level = role.GetRoleLevel(MapID, InstanceID, RoleID)
		        level = math.floor(level/10)
		        role.AddRoleExp(MapID, InstanceID, RoleID, Yuan_Dan_Exp[level]*2)
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 2360) -- "������ʦ��\n    ��ε���ҵ���û�����Ŷ������һЩ������Ϊ�����ɣ�"
				msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
				msg.AddMsgEvent(MsgID, 21, -1) -- ȷ����ť
				msg.AddMsgEvent(MsgID, 1, 21) -- ��ȷ����
				msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
			else
			    if c > 0 then
				    role.RemoveFromRole(MapID, InstanceID, RoleID, 3303212, 1, 420)
					local level = role.GetRoleLevel(MapID, InstanceID, RoleID)
					level = math.floor(level/10)
					role.AddRoleExp(MapID, InstanceID, RoleID, Yuan_Dan_Exp[level])
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 20, 2361) -- "������ʦ��\n    ��ε���ҵ��������ǿǿ�ɡ�������һЩ������Ϊ�����ɣ�"
					msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
					msg.AddMsgEvent(MsgID, 21, -1) -- ȷ����ť
					msg.AddMsgEvent(MsgID, 1, 21) -- ��ȷ����
					msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
				else
				    if d > 0 then
					    local FreeSize = role.GetBagFreeSize(RoleID)
						if FreeSize > 0 then
							role.RemoveFromRole(MapID, InstanceID, RoleID, 3303213, 1, 420)
							role.AddRoleItem(MapID, InstanceID, RoleID, 3303207, 2, 1, 8, 420)
							local MsgID = msg.BeginMsgEvent()
							msg.AddMsgEvent(MsgID, 20, 2362) -- "������ʦ��\n    ��ε���ҵ����̫���ˣ��ٸ���������ҵֽ����ȥ������"
							msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
							msg.AddMsgEvent(MsgID, 21, -1) -- ȷ����ť
							msg.AddMsgEvent(MsgID, 1, 21) -- ��ȷ����
							msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
						else
							local MsgID = msg.BeginMsgEvent()
							msg.AddMsgEvent(MsgID, 20, 2359) -- "������ʦ��\n    ��ı��������ˣ�ȥ����һ�������ɣ�"
							msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
							msg.AddMsgEvent(MsgID, 21, -1) -- ȷ����ť
							msg.AddMsgEvent(MsgID, 1, 21) -- ��ȷ����
							msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
						end
					else
					    local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 20, 2363) -- "������ʦ��\n    �����ҵ�أ���������ȥ����"
						msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
						msg.AddMsgEvent(MsgID, 21, -1) -- ȷ����ť
						msg.AddMsgEvent(MsgID, 1, 21) -- ��ȷ����
						msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
					end
				end
			end
		end
    elseif TalkIndex == 5 then
	    local k = nil
		for i = 0, 4 do
		    if act.GetActScriptData(77, 1, i) ~= nil and act.GetActScriptData(77, 1, i) > 0 then
			    k = i
			end
		end
		if k == nil then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 2364) -- "������ʦ��\n    ���ڰ��ﻹû��ͬѧ��ù��׷�ñŶ��"
			msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
			msg.AddMsgEvent(MsgID, 21, -1) -- ȷ����ť
			msg.AddMsgEvent(MsgID, 1, 21) -- ��ȷ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
		elseif k == 0 then
		    local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 2365) -- "������ʦ��\n    ���ڰ������׷�ñ����ͬѧ�ǣ�\n    XXX        XXX���׷�ñ
			msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
			msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(77, 1, 0))
			msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(77, 1, 5))
			msg.AddMsgEvent(MsgID, 21, -1) -- ȷ����ť
			msg.AddMsgEvent(MsgID, 1, 21) -- ��ȷ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
		elseif k == 1 then
		    local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 2366) -- "������ʦ��\n    ���ڰ������׷�ñ����ͬѧ�ǣ�\n    XXX        XXX���׷�ñ\n    XXX        XXX���׷�ñ
			msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
			msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(77, 1, 0))
			msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(77, 1, 5))
			msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(77, 1, 1))
			msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(77, 1, 6))
			msg.AddMsgEvent(MsgID, 21, -1) -- ȷ����ť
			msg.AddMsgEvent(MsgID, 1, 21) -- ��ȷ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
		elseif k == 2 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 2367) -- "������ʦ��\n    ���ڰ������׷�ñ����ͬѧ�ǣ�\n    XXX        XXX���׷�ñ\n    XXX        XXX���׷�ñ\n    XXX        XXX���׷�ñ
			msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
			msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(77, 1, 0))
			msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(77, 1, 5))
			msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(77, 1, 1))
			msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(77, 1, 6))
			msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(77, 1, 2))
			msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(77, 1, 7))
			msg.AddMsgEvent(MsgID, 21, -1) -- ȷ����ť
			msg.AddMsgEvent(MsgID, 1, 21) -- ��ȷ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
		elseif k == 3 then
		    local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 2368) -- "������ʦ��\n    ���ڰ������׷�ñ����ͬѧ�ǣ�\n    XXX        XXX���׷�ñ\n    XXX        XXX���׷�ñ\n    XXX        XXX���׷�ñ\n    XXX        XXX���׷�ñ
			msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
			msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(77, 1, 0))
			msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(77, 1, 5))
			msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(77, 1, 1))
			msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(77, 1, 6))
			msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(77, 1, 2))
			msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(77, 1, 7))
			msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(77, 1, 3))
			msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(77, 1, 8))
			msg.AddMsgEvent(MsgID, 21, -1) -- ȷ����ť
			msg.AddMsgEvent(MsgID, 1, 21) -- ��ȷ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
		elseif k == 4 then
		    local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 2369) -- "������ʦ��\n    ���ڰ������׷�ñ����ͬѧ�ǣ�\n    XXX        XXX���׷�ñ\n    XXX        XXX���׷�ñ\n    XXX        XXX���׷�ñ\n    XXX        XXX���׷�ñ\n    XXX        XXX���׷�ñ
			msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
			msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(77, 1, 0))
			msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(77, 1, 5))
			msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(77, 1, 1))
			msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(77, 1, 6))
			msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(77, 1, 2))
			msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(77, 1, 7))
			msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(77, 1, 3))
			msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(77, 1, 8))
            msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(77, 1, 4))
			msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(77, 1, 9))
			msg.AddMsgEvent(MsgID, 21, -1) -- ȷ����ť
			msg.AddMsgEvent(MsgID, 1, 21) -- ��ȷ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
		end
    end
end

aux.RegisterCreatureEvent(4900440, 7, "n4900440_OnTalk")

