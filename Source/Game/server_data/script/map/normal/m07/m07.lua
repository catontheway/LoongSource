
--����m07ʱ����ȫ�ֱ���FeiSheng_trigger=1����ˢ�·���NPC
function m07_OnInit(MapID,InstanceID,CreatureID,CreatureTypeID)
	if FeiSheng_trigger == 1 then
		map.MapCreateColCreature(MapID, InstanceID, 4900701, feisheng_fumo.x, feisheng_fumo.y, feisheng_fumo.z, 1, "")
		map.MapCreateColCreature(MapID, InstanceID, 4900702, feisheng_baimei.x, feisheng_baimei.y, feisheng_baimei.z, 1, "")
	end
end
aux.RegisterMapEvent("m07", 0, "m07_OnInit")

